#include "Window.h"


#include <stdexcept>

Window::Window(const int& screenWidth, const int& screenHeight, Camera* camera)
{
    m_camera = camera;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(screenWidth, screenHeight, "Aquarium", nullptr, nullptr);

    if (m_window == nullptr)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(m_window);
    glfwSetWindowUserPointer(m_window, this);

    glfwSetFramebufferSizeCallback(m_window, FrameBuffer_Size_Callback);
    glfwSetCursorPosCallback(m_window, Mouse_Callback);
    glfwSetScrollCallback(m_window, Scroll_Callback);
}

Window::~Window()
{
    delete m_camera;
    glfwTerminate();
}

GLFWwindow* Window::GetWindow() const
{
    return m_window;
}

Camera* Window::GetCamera() const
{
    return m_camera;
}

void Window::ProcessInput(const float deltaTime) const
{
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, true);
    if (glfwGetKey(m_window, GLFW_KEY_R) == GLFW_PRESS) 
    {
        int width, height;
        glfwGetWindowSize(m_window, &width, &height);
        m_camera->Reset(width, height);
    }


    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
    	{m_camera->ProcessKeyboard(Camera::Forward, deltaTime);}
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
        m_camera->ProcessKeyboard(Camera::Backward, deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
        m_camera->ProcessKeyboard(Camera::Left, deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
        m_camera->ProcessKeyboard(Camera::Right, deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
        m_camera->ProcessKeyboard(Camera::Up, deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        m_camera->ProcessKeyboard(Camera::Down, deltaTime);

    
}

