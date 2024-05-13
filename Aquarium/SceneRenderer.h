#pragma once

#include <string>
#include <vector>
#include <functional>
#include "Model.h"
#include "Shader.h"
#include "Textures.h"

class SceneRenderer
{
public:
    SceneRenderer(const std::string& projectPath);
    ~SceneRenderer() = default;
    void Init();
    void Render(const Shader& shader, float deltaTime) const;
    void RenderWater(const Shader& shader, float deltaTime);

private:
    void CreateProjectPath(const string& string);

private:
    void RenderFloor(const Shader& shader, float deltaTime);
    void RenderCeiling(const Shader& shader, float deltaTime);
    void RenderWalls(const Shader& shader, float deltaTime);
    void RenderAquarium(const Shader& shader, float deltaTime);
    void RenderFish(const Shader& shader, float deltaTime);
    void RenderCoralFish(const Shader& shader, float deltaTime);
    void RenderStarfish(const Shader& shader, float deltaTime);

private:
    unsigned int m_ceilingVao{ 0 };
    unsigned int m_ceilingVbo{ 0 };
    unsigned int m_planeVao{ 0 };
    unsigned int m_planeVbo{ 0 };
    unsigned int m_wallVao{ 0 };
    unsigned int m_wallVbo{ 0 };
    unsigned int m_aquariumVao{ 0 };
    unsigned int m_aquariumVbo{ 0 };
    unsigned int m_fishVao{ 0 };
    unsigned int m_fishVbo{ 0 };
    unsigned int m_coralFishVao{ 0 };
    unsigned int m_coralFishVbo{ 0 };
    unsigned int m_starfishVao{ 0 };
    unsigned int m_starfishVbo{ 0 };

private:
    std::string m_projectPath;
    std::vector<std::function<void(const Shader& shader, float deltaTime)>> m_renderers;
    Textures m_textures;
    std::unordered_map<std::string,Model> m_models;
    Model m_fishModel;
    Model m_aquariumModel;
    Model m_coralFishModel;
    Model m_starfishModel;
};
