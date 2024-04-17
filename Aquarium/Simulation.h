#pragma once
#include <string>

#include "Camera.h"
#include "Window.h"

#include <cstdlib>
#include <math.h> 
#include <stdio.h>

#pragma comment (lib, "glfw3dll.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "OpenGL32.lib")

using std::string;


class Simulation
{
public:
	Simulation(const string& fullExePath);

	Camera* GetCamera() const;

private:
	void SetExeLocation(const string& fullExePath);
	void InitializeWindow();

private:
	

private:
	static constexpr int SCREEN_WIDTH = 1366;
	static constexpr int SCREEN_HEIGHT = 768;

private:
	Camera* m_camera;
	Window* m_window	;
	string m_exeLocation;
};
