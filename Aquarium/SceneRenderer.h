#pragma once
#include <functional>
#include "Shader.h"
#include "Textures.h"


class SceneRenderer
{
public:
	SceneRenderer(const string& projectPath);
	~SceneRenderer() = default;
	void Init();
	void Render(const Shader& shader, float deltaTime) const;

private:
	void RenderFloor(const Shader& shader, float deltaTime);
	void RenderCeiling(const Shader& shader, float deltaTime);
	void RenderWalls(const Shader& shader, float deltaTime);

private:
	unsigned int m_ceilingVao{ 0 };
	unsigned int m_ceilingVbo{ 0 };
	unsigned int m_planeVao{ 0 };
	unsigned int m_planeVbo{ 0 };
	unsigned int m_cubeVao{ 0 };
	unsigned int m_cubeVbo{ 0 };
	unsigned int m_wallVao{ 0 };
	unsigned int m_wallVbo{ 0 };
private:
	std::vector<std::function<void(const Shader& shader, float deltaTime)>> m_renderers;
	Textures m_textures;
	
};
