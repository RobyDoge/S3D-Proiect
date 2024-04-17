#pragma once
#include <glfw3.h>

#include "Simulation.h"

class Window
{
public:
	Window(const int& screenWidth, const int& screenHeight);
	auto ViewportDidResize(int w, int h) -> void;
	auto RenderScene(void) -> void;
	auto UpdateScene(double ms) -> void;

private:
	inline static void FrameBuffer_Size_Callback(GLFWwindow* window, const int width, const int height)
	{
		const auto simulation = static_cast<Simulation*>(glfwGetWindowUserPointer(window));
		const auto camera = simulation->GetCamera();
		camera->Reshape(width, height);
	}
	inline static void Mouse_Callback(GLFWwindow* window, const double xPos, const double yPos)
	{
		const auto simulation = static_cast<Simulation*>(glfwGetWindowUserPointer(window));
		const auto camera = simulation->GetCamera();
		camera->MouseControl(static_cast<float>(xPos), static_cast<float>(yPos));
	}
	inline static void Scroll_Callback(GLFWwindow* window, const double xOffset, const double yOffset)
	{
		const auto simulation = static_cast<Simulation*>(glfwGetWindowUserPointer(window));
		const auto camera = simulation->GetCamera();
		camera->ProcessMouseScroll(static_cast<float>(yOffset));
	}

private:
	GLFWwindow* m_window;
};
