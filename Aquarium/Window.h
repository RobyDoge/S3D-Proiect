#pragma once
#include <glew.h>
#include <glfw3.h>
#include "Camera.h"


class Window
{
public:
	Window(const int& screenWidth, const int& screenHeight, Camera* camera);
	~Window();
	GLFWwindow* GetWindow() const;
	Camera* GetCamera() const;
	void ProcessInput(const float deltaTime) const;

private:
	inline static void FrameBuffer_Size_Callback(GLFWwindow* win, const int width, const int height)
	{
		const auto window = static_cast<Window*>(glfwGetWindowUserPointer(win));
		window->m_camera->Reshape(width, height);
	}
	inline static void Mouse_Callback(GLFWwindow* win, const double xPos, const double yPos)
	{
		if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS)
		{
			++m_movingSegment;
			return;
		}
		const auto window = static_cast<Window*>(glfwGetWindowUserPointer(win));
		window->m_camera->MouseControl(static_cast<float>(xPos), static_cast<float>(yPos),m_movingSegment);
	}
	inline static void Scroll_Callback(GLFWwindow* win, const double xOffset, const double yOffset)
	{
		const auto window = static_cast<Window*>(glfwGetWindowUserPointer(win));
		window->m_camera->ProcessMouseScroll(static_cast<float>(yOffset));
	}

private:
	inline static int m_movingSegment{ 0 };
	GLFWwindow* m_window;
	Camera* m_camera;
};