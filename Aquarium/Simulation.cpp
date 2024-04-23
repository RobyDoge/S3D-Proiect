#include <glew.h>
#include "Simulation.h"


#include <iostream>
#include <format>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>



Simulation::Simulation(const string& fullExePath)
{
	SetExeLocation(fullExePath);
	m_camera = new Camera(SCREEN_WIDTH, SCREEN_HEIGHT, glm::vec3(0.0, 1.0, 3.0));
	InitializeWindow();
	/*InitializeGLEW();
    CreateShaders();*/

}

Simulation::~Simulation()
{
	delete m_camera;
	delete m_window;
}


Camera* Simulation::GetCamera() const
{
	return m_camera;
}

void Simulation::AddTexture(const string& textureName, const string& texturePath)
{
    m_textures[textureName] = CreateTexture(m_exeLocation+ texturePath);
}

void Simulation::AddMultipleTextures(const std::unordered_map<string, string>& textureName_texturePath)
{
	for (const auto& [textureName, texturePath] : textureName_texturePath)
	{
		m_textures[textureName] = CreateTexture(texturePath);
	}
}

void Simulation::Run()
{
    double lastFrame = 0.0f;

    while (!glfwWindowShouldClose(m_window->GetWindow()))
    {
        // per-frame time logic
        // --------------------
        const double currentFrame = glfwGetTime();
        m_deltaTime = static_cast<float>(currentFrame - lastFrame);
        lastFrame = currentFrame;


        glm::vec3 lightPos(2, 0.f, 3);
        

        // input
        // -----
        ProcessInput();

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
        m_shadowMappingDepthShader.Use();
        m_shadowMappingDepthShader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFbo);
        glClear(GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_textures.at("floorTexture"));
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        m_window->RenderScene(m_shadowMappingDepthShader);
        glCullFace(GL_BACK);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // reset viewport
        glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 2. render scene as normal using the generated depth/shadow map 
        glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_shadowMappingShader.Use();
        glm::mat4 projection = m_window->GetCamera()->GetProjectionMatrix();
        glm::mat4 view = m_window->GetCamera()->GetViewMatrix();
        m_shadowMappingShader.SetMat4("projection", projection);
        m_shadowMappingShader.SetMat4("view", view);
        // set light uniforms
        m_shadowMappingShader.SetVec3("viewPos", m_window->GetCamera()->GetPosition());
        m_shadowMappingShader.SetVec3("lightPos", lightPos);
        m_shadowMappingShader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_textures.at("floorTexture"));
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_depthMap);
        glDisable(GL_CULL_FACE);
        m_window->RenderScene(m_shadowMappingShader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(m_window->GetWindow());
        glfwPollEvents();
    }
}

void Simulation::SetExeLocation(const string& fullExePath)
{
	if (const size_t last_slash_idx = fullExePath.rfind('\\'); 
		std::string::npos != last_slash_idx)
	{
		m_exeLocation = fullExePath.substr(0, last_slash_idx);
	}
}

void Simulation::InitializeWindow()
{
	m_window = new Window(SCREEN_WIDTH, SCREEN_HEIGHT, m_camera);
}

void Simulation::InitializeGLEW()
{
	glewInit();
	glEnable(GL_DEPTH_TEST);
    glGenFramebuffers(1, &m_depthMapFbo);
    // create depth texture
    glGenTextures(1, &m_depthMap);
    glBindTexture(GL_TEXTURE_2D, m_depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	constexpr float borderColor[] = { 0.0, 0.0, 0.0, 0.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    // shader configuration
    // --------------------
    

    // lighting info
    // -------------
    glEnable(GL_CULL_FACE);
}

void Simulation::CreateShaders()
{
	m_shadowMappingShader = { "ShadowMapping.vs", "ShadowMapping.fs" };
	m_shadowMappingDepthShader = {"ShadowMappingDepth.vs", "ShadowMappingDepth.fs"};

    
}

unsigned Simulation::CreateTexture(const string& texturePath)
{
    unsigned int textureId = -1;

    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
    if (!data) throw std::runtime_error(std::format("Failed to load texture: {}",texturePath));
    GLenum format = GL_RED;
    if (nrChannels == 1)
	    format = GL_RED;
    else if (nrChannels == 3)
	    format = GL_RGB;
    else if (nrChannels == 4)
	    format = GL_RGBA;


    if (!data) {
        std::cerr << "Failed to load texture: " << texturePath << std::endl;
        throw std::runtime_error("Failed to load texture");
    }

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    
    stbi_image_free(data);
    return textureId;
}

void Simulation::ProcessInput()
{
    
    if (glfwGetKey(m_window->GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window->GetWindow(), true);

    if (glfwGetKey(m_window->GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
        m_camera->ProcessKeyboard(Camera::Forward, m_deltaTime);
    if (glfwGetKey(m_window->GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
        m_camera->ProcessKeyboard(Camera::Backward, m_deltaTime);
    if (glfwGetKey(m_window->GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
        m_camera->ProcessKeyboard(Camera::Left, m_deltaTime);
    if (glfwGetKey(m_window->GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
        m_camera->ProcessKeyboard(Camera::Right, m_deltaTime);
    if (glfwGetKey(m_window->GetWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
        m_camera->ProcessKeyboard(Camera::Up, m_deltaTime);
    if (glfwGetKey(m_window->GetWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        m_camera->ProcessKeyboard(Camera::Down, m_deltaTime);

    if (glfwGetKey(m_window->GetWindow(), GLFW_KEY_R) == GLFW_PRESS) {
        int width, height;
        glfwGetWindowSize(m_window->GetWindow(), &width, &height);
        m_camera->Reset(width, height);
    }
}

void Simulation::RenderScene(Shader shader)
{
    m_window->RenderScene(shader);
    return;
        // floor
        glm::mat4 model;
        //shader.SetMat4("model", model);
        //renderFloor();

        ////ceiling
        //glm::mat4 model1;
        //shader.SetMat4("model1", model1);
        //renderCeiling();


        // cube
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(0.0f, 1.75f, 0.0));
        model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
        model = glm::scale(model, glm::vec3(0.75f));
        shader.SetMat4("model", model);
        RenderCube();

        model = glm::mat4();
        model = glm::translate(model, glm::vec3(.0f, .75f, 1.0));
        model = glm::scale(model, glm::vec3(.5f));
        model = glm::rotate(model, glm::radians(20.0f), glm::normalize(glm::vec3(1.0, 1.0, 0)));
        shader.SetMat4("model", model);
        RenderCube();

        model = glm::mat4();
        model = glm::translate(model, glm::vec3(1.3f, 2.f, 0.75f));
        model = glm::scale(model, glm::vec3(.25f));
        model = glm::rotate(model, glm::radians(50.0f), glm::normalize(glm::vec3(0, 1.0, 1.0)));
        shader.SetMat4("model", model);
        RenderCube();
    
}
void Simulation::RenderCube()
{
    // initialize (if necessary)
    if (cubeVAO == 0)
    {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
            1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
            1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
            1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
            1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
            1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
            1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
            1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
            1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
            1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
            1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

GLFWwindow* Simulation::GetWindow()
{
	return m_window->GetWindow();
}
