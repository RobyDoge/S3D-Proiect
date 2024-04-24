#pragma once
#include <functional>
#include "Shader.h"

class SceneRenderer
{
public:
	SceneRenderer() = default;
	~SceneRenderer() = default;
	void Init();
	void Render(const Shader& shader, float deltaTime) const;

private:
	void RenderFloor();
	void RenderCube();
	void RenderCeiling();

private:
	unsigned int m_ceilingVao{ 0 };
	unsigned int m_ceilingVbo{ 0 };
	unsigned int m_planeVao{ 0 };
	unsigned int m_planeVbo{ 0 };
	unsigned int m_cubeVao{ 0 };
	unsigned int m_cubeVbo{ 0 };
private:
	std::vector<std::function<void()>> m_renderers;
	
};
