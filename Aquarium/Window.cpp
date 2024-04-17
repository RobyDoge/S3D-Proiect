#include "Window.h"
#include <stdexcept>

Window::Window(const int& screenWidth,const int& screenHeight)
{
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
auto Window::ViewportDidResize(int w, int h) -> void
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
}