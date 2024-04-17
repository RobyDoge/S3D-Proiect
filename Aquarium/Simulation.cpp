#include "Simulation.h"
#include <iostream>
#include <vec3.hpp>


Simulation::Simulation(const string& fullExePath)
{
	SetExeLocation(fullExePath);
	m_camera = new Camera(SCREEN_WIDTH, SCREEN_HEIGHT, glm::vec3(0.0, 1.0, 3.0));
	InitializeWindow();

}

Camera* Simulation::GetCamera() const
{
	return m_camera;
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
	m_window = new Window(SCREEN_WIDTH, SCREEN_HEIGHT);
}
