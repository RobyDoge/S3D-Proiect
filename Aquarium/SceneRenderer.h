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
    void UpdateFishPosition(float deltaTime, glm::mat4& model, float speed);

private:
    void RenderFloor(const Shader& shader, float deltaTime);
    void RenderCeiling(const Shader& shader, float deltaTime);
    void RenderWalls(const Shader& shader, float deltaTime);
    void RenderAquariumFloor(const Shader& shader, float deltaTime);
    void RenderFish(const Shader& shader, float deltaTime);
    void RenderCoralFish(const Shader& shader, float deltaTime);
    void RenderCastle(const Shader& shader, float deltaTime);
    void RenderSkull(const Shader& shader, float deltaTime);
    void RenderBlueGreenFish(const Shader& shader, float deltaTime);
    void RenderRedCoral(const Shader& shader, float deltaTime);
    void RenderBlueTangFish(const Shader& shader, float deltaTime);
    void RenderCat(const Shader& shader, float deltaTime);
    void RenderDaffodils(const Shader& shader, float deltaTime);
    void RenderBush(const Shader& shader, float deltaTime);

private:
    unsigned int m_ceilingVao{ 0 };
    unsigned int m_ceilingVbo{ 0 };
    unsigned int m_planeVao{ 0 };
    unsigned int m_planeVbo{ 0 };
    unsigned int m_wallVao{ 0 };
    unsigned int m_wallVbo{ 0 };
    unsigned int m_aquariumVao{ 0 };
    unsigned int m_aquariumVbo{ 0 };
    unsigned int m_aquariumFloorVbo{ 0 };
    unsigned int m_aquariumFloorVao{ 0 };
private:
    float m_fishAngle{ 10000 };
    float m_catAngle{ 0.0f };
private:
    std::string m_projectPath;
    std::vector<std::function<void(const Shader& shader, float deltaTime)>> m_renderers;
    Textures m_textures;
    std::unordered_map<std::string,Model> m_models;
     
};
