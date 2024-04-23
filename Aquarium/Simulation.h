#pragma once
#include <string>

#include "Camera.h"
#include "Window.h"
#include "Shader.h"

#include <cstdlib>
#include <math.h> 
#include <stdio.h>
#include <unordered_map>



using std::string;


class Simulation
{
public:
	Simulation(const string& fullExePath);
	~Simulation();
	Camera* GetCamera() const;
	void AddTexture(const string& textureName, const string& texturePath);
	void AddMultipleTextures(const std::unordered_map<string,string>& textureName_texturePath);
	void Run();
	GLFWwindow* GetWindow();

public:
	void SetExeLocation(const string& fullExePath);
	void InitializeWindow();
	void InitializeGLEW();
	void CreateShaders();
	static unsigned int CreateTexture(const string& texturePath);
	void ProcessInput();
	void RenderScene(Shader shader);
	void RenderCube();
private:
	

public:
	static constexpr int SCREEN_WIDTH = 1366;
	static constexpr int SCREEN_HEIGHT = 768;
	static constexpr unsigned int SHADOW_WIDTH = 1024;
	static constexpr unsigned int SHADOW_HEIGHT = 1024;
public:
	Camera* m_camera;
	Window* m_window;
	string m_exeLocation;
	Shader m_shadowMappingShader;
	Shader m_shadowMappingDepthShader;
	std::unordered_map<string, unsigned int> m_textures;  // Texture name and texture ID
	float m_deltaTime= 0.0f;
	unsigned int m_depthMap;
	unsigned int m_depthMapFbo;
	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;
};
