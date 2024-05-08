#include "Camera.h"
#include "Shader.h"
#include "Window.h"
#include "SceneRenderer.h"

#pragma comment (lib, "glfw3dll.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "OpenGL32.lib")


constexpr unsigned int SCREEN_WIDTH {1920};
constexpr unsigned int SCREEN_HEIGHT{1080};

int main(int argc, char** argv)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const auto camera = new Camera(SCREEN_WIDTH, SCREEN_HEIGHT, glm::vec3(0.0, 0.0, 0.0));
    const auto window = new Window{ SCREEN_WIDTH, SCREEN_HEIGHT,camera };
    
    float lastFrame = 0.0f;

    glewInit();
    glEnable(GL_DEPTH_TEST);

    const Shader shadowMappingShader("Shaders\\ShadowMapping.vs", "Shaders\\ShadowMapping.fs");
    const Shader shadowMappingDepthShader("Shaders\\ShadowMappingDepth.vs", "Shaders\\ShadowMappingDepth.fs");

    // create a framebuffer object + textures
    SceneRenderer scene_renderer = { argv[0] };
    scene_renderer.Init();

    // configure depth map FBO
    // -----------------------
    constexpr unsigned int shadow_width = 1024;
	constexpr unsigned int shadow_height = 1024;
    unsigned int depthMapFbo;
    glGenFramebuffers(1, &depthMapFbo);


	// create depth texture
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadow_width, shadow_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    constexpr float borderColor[] = { 0.0, 0.0, 0.0, 0.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    // shader configuration
    // --------------------
    shadowMappingShader.Use();
    shadowMappingShader.SetInt("diffuseTexture", 0);
    shadowMappingShader.SetInt("shadowMap", 1);

    // lighting info
    // -------------
    constexpr glm::vec3 lightPos(0.0f, 5.0f, 0.0f);


    glEnable(GL_CULL_FACE);
    while (!glfwWindowShouldClose(window->GetWindow()))
    {
	    const auto currentFrame = static_cast<float>(glfwGetTime());
	    const auto deltaTime = currentFrame - lastFrame;
        window->ProcessInput(deltaTime);
    	lastFrame = currentFrame;
		
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 1. render depth of scene to texture (from light's perspective)
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
	    constexpr float near_plane = 1.0f;
	    constexpr float far_plane = 7.5f;
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        lightView = lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 0.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;

        // render scene from light's point of view
        shadowMappingDepthShader.Use();
        shadowMappingDepthShader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);


        glViewport(0, 0, shadow_width, shadow_height);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFbo);
        glClear(GL_DEPTH_BUFFER_BIT);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        scene_renderer.Render(shadowMappingDepthShader, deltaTime);
        glCullFace(GL_BACK);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // reset viewport
        glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        // 2. render scene as normal using the generated depth/shadow map 
        glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shadowMappingShader.Use();
        glm::mat4 projection = camera->GetProjectionMatrix();
        glm::mat4 view = camera->GetViewMatrix();
        shadowMappingShader.SetMat4("projection", projection);
        shadowMappingShader.SetMat4("view", view);
        // set light uniforms
        shadowMappingShader.SetVec3("viewPos", camera->GetPosition());
        shadowMappingShader.SetVec3("lightPos", lightPos);
        shadowMappingShader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glDisable(GL_CULL_FACE);
        scene_renderer.Render(shadowMappingShader, deltaTime);

        glfwSwapBuffers(window->GetWindow());
        glfwPollEvents();
    }

    delete camera;
    glfwTerminate();
    return 0;
}