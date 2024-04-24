//Added by Us:
#include "Camera.h"
#include "Shader.h"
#include "Window.h"
#include "Textures.h"
#include "SceneRenderer.h"


//Already Present:
#include <cstdlib>
#include <math.h> 
#include <stdio.h>



#include <fstream>
#include <iostream>
#include <sstream>



#pragma comment (lib, "glfw3dll.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "OpenGL32.lib")


constexpr unsigned int SCREEN_WIDTH {1920};
constexpr unsigned int SCREEN_HEIGHT{1080};


void renderScene(const Shader& shader,float deltaTime);
//void RenderCube();
//void RenderFloor();
//void RenderCeiling();

SceneRenderer scene_renderer;


int main(int argc, char** argv)
{
    

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const auto camera = new Camera(SCREEN_WIDTH, SCREEN_HEIGHT, glm::vec3(0.0, 0.0, 0.0));
    const auto window = new Window{ SCREEN_WIDTH, SCREEN_HEIGHT,camera };
    scene_renderer.Init();
    float lastFrame = 0.0f;

    glewInit();
    glEnable(GL_DEPTH_TEST);

    const Shader shadowMappingShader("ShadowMapping.vs", "ShadowMapping.fs");
    const Shader shadowMappingDepthShader("ShadowMappingDepth.vs", "ShadowMappingDepth.fs");

    // load textures
    // -------------
    Textures textures{ argv[0] };
    textures.AddTexture("Floor", "Floor.jpg");


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
    constexpr glm::vec3 lightPos(0.0f, 2.0f, 0.0f);


    glEnable(GL_CULL_FACE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window->GetWindow()))
    {
	    const auto currentFrame = static_cast<float>(glfwGetTime());
	    const auto deltaTime = currentFrame - lastFrame;
        window->ProcessInput(deltaTime);
    	lastFrame = currentFrame;
		

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 1. render depth of scene to texture (from light's perspective)
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 1.0f, far_plane = 7.5f;
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 0.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;

        // render scene from light's point of view
        shadowMappingDepthShader.Use();
        shadowMappingDepthShader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);

        glViewport(0, 0, shadow_width, shadow_height);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFbo);
        glClear(GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures.GetTexture("Floor"));
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        renderScene(shadowMappingDepthShader, deltaTime);
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
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures.GetTexture("Floor"));
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glDisable(GL_CULL_FACE);
        renderScene(shadowMappingShader, deltaTime);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window->GetWindow());
        glfwPollEvents();
    }

    delete camera;
    glfwTerminate();
    return 0;
}



// renders the 3D scene
// --------------------
void renderScene(const Shader& shader, const float deltaTime)
{
    //now:
    scene_renderer.Render(shader,deltaTime);




    //before:
 //   // floor
 //   glm::mat4 model;
 //   shader.SetMat4("model", model);

	//RenderFloor();
 //   RenderCeiling();


 //   // cube
 //   model = glm::mat4();
 //   model = glm::translate(model, glm::vec3(0.0f, 1.75f, 0.0));
 //   model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
 //   model = glm::scale(model, glm::vec3(0.75f));
 //   shader.SetMat4("model", model);
 //   RenderCube();

 //   model = glm::mat4();
 //   model = glm::translate(model, glm::vec3(.0f, .75f, 1.0));
 //   model = glm::scale(model, glm::vec3(.5f));
 //   model = glm::rotate(model, glm::radians(20.0f), glm::normalize(glm::vec3(1.0, 1.0, 0)));
 //   shader.SetMat4("model", model);
 //   RenderCube();

 //   model = glm::mat4();
 //   model = glm::translate(model, glm::vec3(1.3f, 2.f, 0.75f));
 //   model = glm::scale(model, glm::vec3(.25f));
 //   model = glm::rotate(model, glm::radians(50.0f), glm::normalize(glm::vec3(0, 1.0, 1.0)));
 //   shader.SetMat4("model", model);
 //   RenderCube();
}

//
//unsigned int ceiling_vao = 0;
//void RenderCeiling() {
//
//    if (ceiling_vao == 0) {
//
//        float ceilingVertices[] = {
//            // Pozitii           // Normale         // Coordonate textură
//             5.0f,  5.0f,  5.0f,  0.0f,  1.0f,  0.0f,  5.0f,  0.0f, // Dreapta-sus
//            -5.0f,  5.0f,  5.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, // Stânga-sus
//            -5.0f,  5.0f, -5.0f,  0.0f,  1.0f,  0.0f,  0.0f,  5.0f, // Stânga-jos
//             5.0f,  5.0f,  5.0f,  0.0f,  1.0f,  0.0f,  5.0f,  0.0f, // Dreapta-sus
//            -5.0f,  5.0f, -5.0f,  0.0f,  1.0f,  0.0f,  0.0f,  5.0f, // Stânga-jos
//             5.0f,  5.0f, -5.0f,  0.0f,  1.0f,  0.0f,  5.0f,  5.0f  // Dreapta-jos
//        };
//
//
//
//        glGenVertexArrays(1, &ceiling_vao);
//        unsigned int ceilingVBO;
//        glGenBuffers(1, &ceilingVBO);
//
//
//        glBindVertexArray(ceiling_vao);
//        glBindBuffer(GL_ARRAY_BUFFER, ceilingVBO);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(ceilingVertices), ceilingVertices, GL_STATIC_DRAW);
//
//
//        glEnableVertexAttribArray(0);
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//        glEnableVertexAttribArray(1);
//        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//        glEnableVertexAttribArray(2);
//        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//
//
//        glBindVertexArray(0);
//    }
//
//    glBindVertexArray(ceiling_vao);
//    glDrawArrays(GL_TRIANGLES, 0, 6);
//    glBindVertexArray(0);
//}
//
//
//unsigned int plane_vao = 0;
//void RenderFloor()
//{
//    unsigned int planeVBO;
//
//    if (plane_vao == 0) {
//        // set up vertex data (and buffer(s)) and configure vertex attributes
//        float planeVertices[] = {
//            // positions            // normals         // texcoords
//            25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
//            -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
//            -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
//
//            25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
//            -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
//            25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
//        };
//        // plane VAO
//        glGenVertexArrays(1, &plane_vao);
//        glGenBuffers(1, &planeVBO);
//        glBindVertexArray(plane_vao);
//        glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
//        glEnableVertexAttribArray(0);
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//        glEnableVertexAttribArray(1);
//        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//        glEnableVertexAttribArray(2);
//        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//        glBindVertexArray(0);
//    }
//
//    glBindVertexArray(plane_vao);
//    glDrawArrays(GL_TRIANGLES, 0, 6);
//}
//
//
//// renderCube() renders a 1x1 3D cube in NDC.
//// -------------------------------------------------
//unsigned int cube_vao = 0;
//unsigned int cube_vbo = 0;
//void RenderCube()
//{
//    // initialize (if necessary)
//    if (cube_vao == 0)
//    {
//        float vertices[] = {
//            // back face
//            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
//            1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
//            1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
//            1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
//            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
//            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
//            // front face
//            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
//            1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
//            1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
//            1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
//            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
//            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
//            // left face
//            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
//            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
//            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
//            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
//            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
//            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
//            // right face
//            1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
//            1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
//            1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
//            1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
//            1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
//            1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
//            // bottom face
//            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
//            1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
//            1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
//            1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
//            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
//            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
//            // top face
//            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
//            1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
//            1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
//            1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
//            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
//            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
//        };
//        glGenVertexArrays(1, &cube_vao);
//        glGenBuffers(1, &cube_vbo);
//        // fill buffer
//        glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//        // link vertex attributes
//        glBindVertexArray(cube_vao);
//        glEnableVertexAttribArray(0);
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//        glEnableVertexAttribArray(1);
//        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//        glEnableVertexAttribArray(2);
//        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//        glBindVertexArray(0);
//    }
//    // render Cube
//    glBindVertexArray(cube_vao);
//    glDrawArrays(GL_TRIANGLES, 0, 36);
//    glBindVertexArray(0);
//}
