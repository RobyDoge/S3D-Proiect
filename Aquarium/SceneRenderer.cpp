#include "SceneRenderer.h"
#include <glew.h>
#include <memory>
#include <string>


#include <GLM.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>


SceneRenderer::SceneRenderer(const string& projectPath) :m_textures{ projectPath }
{
    m_textures.AddTexture("Floor", "Floor.jpg");
    m_textures.AddTexture("Ceiling", "Ceiling.jpg");
    m_textures.AddTexture("Walls", "Walls.jpg");
    m_textures.AddTexture("Water", "glass.png");
    m_textures.AddTexture("AFloor", "AquariumFloor.jpg");
    CreateProjectPath(projectPath);

    m_models.insert({ "clownFish", { m_projectPath + "\\Models\\Fish\\12265_Fish_v1_L2.obj", false, false } });
    m_models.insert({ "coralFish",{m_projectPath + "\\Models\\Coral_Beauty_Angelfish\\13009_Coral_Beauty_Angelfish_v1_l3.obj", false, false} });
    m_models.insert({"castle", { m_projectPath + "\\Models\\Aquarium_Castle\\13020_Aquarium_Castle_v1_L1.obj",false,false }});
    m_models.insert({ "skull",{ m_projectPath + "\\Models\\Aquarium_Skull\\13022_Aquarium_Skull_v1_L1.obj",false,false } });
    m_models.insert({ "blueGreenFish", { m_projectPath + "\\Models\\BlueGreen_Fish\\13007_Blue-Green_Reef_Chromis_v2_l3.obj",false,false } });
    m_models.insert({ "redCoral", { m_projectPath + "\\Models\\Red_Coral\\10010_Coral_v1_L3.obj",false,false } });
    m_models.insert({ "blueFish", { m_projectPath + "\\Models\\BlueTang_Fish\\13006_Blue_Tang_v1_l3.obj",false,false } });
    m_models.insert({ "cat", { m_projectPath + "\\Models\\Cat\\12221_Cat_v1_l3.obj",false,false } });
    m_models.insert({ "daffodilModel", { m_projectPath + "\\Models\\Daffodil\\12977_Daffodil_flower_v1_l2.obj",false,false } });
}

void SceneRenderer::Init()
{
    m_renderers.emplace_back([this](const Shader& shader, const float deltaTime) {RenderCeiling(shader, deltaTime); });
    m_renderers.emplace_back([this](const Shader& shader, const float deltaTime) { RenderFloor(shader, deltaTime); });
    m_renderers.emplace_back([this](const Shader& shader, const float deltaTime) {RenderAquariumFloor(shader, deltaTime); });
    m_renderers.emplace_back([this](const Shader& shader, const float deltaTime) { RenderWalls(shader, deltaTime); });
    m_renderers.emplace_back([this](const Shader& shader, const float deltaTime) {RenderFish(shader, deltaTime); });
    m_renderers.emplace_back([this](const Shader& shader, const float deltaTime) {RenderCoralFish(shader, deltaTime); });
    m_renderers.emplace_back([this](const Shader& shader, const float deltaTime) {RenderCastle(shader, deltaTime); });
    m_renderers.emplace_back([this](const Shader& shader, const float deltaTime) {RenderSkull(shader, deltaTime); });
    m_renderers.emplace_back([this] (const Shader& shader, const float deltaTime) { RenderBlueGreenFish(shader, deltaTime); });
    m_renderers.emplace_back([this](const Shader& shader, const float deltaTime) {RenderRedCoral(shader, deltaTime); });
    m_renderers.emplace_back([this](const Shader& shader, const float deltaTime) {RenderBlueTangFish(shader, deltaTime); });
    m_renderers.emplace_back([this](const Shader& shader, const float deltaTime) {RenderCat(shader, deltaTime); });
    m_renderers.emplace_back([this](const Shader& shader, const float deltaTime) {RenderDaffodils(shader, deltaTime); });
}

void SceneRenderer::Render(const Shader& shader, const float deltaTime) const
{
    constexpr glm::mat4 model{ 1 };
    shader.SetMat4("model", model);
    for (const auto& render : m_renderers)
    {
        render(shader, deltaTime);
    }
}


void SceneRenderer::RenderWater(const Shader& shader, float deltaTime)
{
    if (m_aquariumVao == 0) {
        constexpr float scaleZ = 1.5;
        constexpr float scaleY = 1;
        constexpr float scaleX = 1.5;
        // Define vertex data for a cube
        constexpr float aquariumVertices[] = {
            // Poziții scalate cu factorul scale    // Normale scalate         // Coordonate textură
            -1.0f * scaleX, -1.0f * scaleY, -1.0f * scaleZ,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
             1.0f * scaleX, -1.0f * scaleY, -1.0f * scaleZ,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f,
             1.0f * scaleX,  1.0f * scaleY, -1.0f * scaleZ,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
             1.0f * scaleX,  1.0f * scaleY, -1.0f * scaleZ,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
            -1.0f * scaleX,  1.0f * scaleY, -1.0f * scaleZ,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f,
            -1.0f * scaleX, -1.0f * scaleY, -1.0f * scaleZ,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,

            -1.0f * scaleX, -1.0f * scaleY,  1.0f * scaleZ,  0.0f, 0.0f,  1.0f,  0.0f, 0.0f,
             1.0f * scaleX, -1.0f * scaleY,  1.0f * scaleZ,  0.0f, 0.0f,  1.0f,  1.0f, 0.0f,
             1.0f * scaleX,  1.0f * scaleY,  1.0f * scaleZ,  0.0f, 0.0f,  1.0f,  1.0f, 1.0f,
             1.0f * scaleX,  1.0f * scaleY,  1.0f * scaleZ,  0.0f, 0.0f,  1.0f,  1.0f, 1.0f,
            -1.0f * scaleX,  1.0f * scaleY,  1.0f * scaleZ,  0.0f, 0.0f,  1.0f,  0.0f, 1.0f,
            -1.0f * scaleX, -1.0f * scaleY,  1.0f * scaleZ,  0.0f, 0.0f,  1.0f,  0.0f, 0.0f,

            -1.0f * scaleX,  1.0f * scaleY,  1.0f * scaleZ, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
            -1.0f * scaleX,  1.0f * scaleY, -1.0f * scaleZ, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
            -1.0f * scaleX, -1.0f * scaleY, -1.0f * scaleZ, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
            -1.0f * scaleX, -1.0f * scaleY, -1.0f * scaleZ, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
            -1.0f * scaleX, -1.0f * scaleY,  1.0f * scaleZ, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
            -1.0f * scaleX,  1.0f * scaleY,  1.0f * scaleZ, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,

             1.0f * scaleX,  1.0f * scaleY,  1.0f * scaleZ,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
             1.0f * scaleX,  1.0f * scaleY, -1.0f * scaleZ,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
             1.0f * scaleX, -1.0f * scaleY, -1.0f * scaleZ,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
             1.0f * scaleX, -1.0f * scaleY, -1.0f * scaleZ,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
             1.0f * scaleX, -1.0f * scaleY,  1.0f * scaleZ,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
             1.0f * scaleX,  1.0f * scaleY,  1.0f * scaleZ,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,

            -1.0f * scaleX, 1.0f * scaleY, -1.0f * scaleZ, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            1.0f * scaleX, 1.0f * scaleY, -1.0f * scaleZ, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            1.0f * scaleX, 1.0f * scaleY, 1.0f * scaleZ, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            1.0f * scaleX, 1.0f * scaleY, 1.0f * scaleZ, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -1.0f * scaleX, 1.0f * scaleY, 1.0f * scaleZ, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            -1.0f * scaleX, 1.0f * scaleY, -1.0f * scaleZ, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f

        };


        // Generarea VAO și VBO
        glGenVertexArrays(1, &m_aquariumVao);
        glGenBuffers(1, &m_aquariumVbo);
        glBindVertexArray(m_aquariumVao);
        glBindBuffer(GL_ARRAY_BUFFER, m_aquariumVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(aquariumVertices), aquariumVertices, GL_STATIC_DRAW);

        // Atributul pentru poziție
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), static_cast<void*>(nullptr));
        glEnableVertexAttribArray(0);
        // Atributul pentru normală
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // Atributul pentru coordonate textură
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    constexpr glm::mat4 model{ 1 };
    shader.SetMat4("model", model);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textures.GetTexture("Water"));

    glBindVertexArray(m_aquariumVao);


    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

}

void SceneRenderer::CreateProjectPath(const string& string)
{

    m_projectPath = string.substr(0, string.find_last_of('\\', string.find_last_of(R"(\)") - 1));
    m_projectPath = m_projectPath.substr(0, m_projectPath.find_last_of(R"(\)"));
}

void SceneRenderer::RenderFloor(const Shader& shader, const float deltaTime)
{
    if (m_planeVao == 0) {
        // set up vertex data (and buffer(s)) and configure vertex attributes
        constexpr float planeVertices[] = {
            // positions            // normals         // texcoords
            5.0f, -0.5f, 5.0f, 0.0f, 1.0f, 0.0f, 5.0f, 0.0f,
            -5.0f, -0.5f, 5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            -5.0f, -0.5f, -5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 5.0f,

            5.0f, -0.5f, 5.0f, 0.0f, 1.0f, 0.0f, 5.0f, 0.0f,
            -5.0f, -0.5f, -5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 5.0f,
            5.0f, -0.5f, -5.0f, 0.0f, 1.0f, 0.0f, 5.0f, 5.0f
        };
        // plane VAO
        glGenVertexArrays(1, &m_planeVao);
        glGenBuffers(1, &m_planeVbo);
        glBindVertexArray(m_planeVao);
        glBindBuffer(GL_ARRAY_BUFFER, m_planeVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), static_cast<void*>(nullptr));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
        glBindVertexArray(0);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textures.GetTexture("Floor"));

    glBindVertexArray(m_planeVao);
    glDrawArrays(GL_TRIANGLES, 0, 6);

}

void SceneRenderer::RenderCeiling(const Shader& shader, const float deltaTime)
{
    if (m_ceilingVao == 0) {
        constexpr float ceilingVertices[] = {
            //Pozitii           // Normale         // Coordonate textură
            5.0f,  5.0f,  5.0f,  0.0f,  1.0f,  0.0f,  5.0f,  0.0f, // Dreapta-sus
           -5.0f,  5.0f,  5.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, // Stânga-sus
           -5.0f,  5.0f, -5.0f,  0.0f,  1.0f,  0.0f,  0.0f,  5.0f, // Stânga-jos
            5.0f,  5.0f,  5.0f,  0.0f,  1.0f,  0.0f,  5.0f,  0.0f, // Dreapta-sus
           -5.0f,  5.0f, -5.0f,  0.0f,  1.0f,  0.0f,  0.0f,  5.0f, // Stânga-jos
            5.0f,  5.0f, -5.0f,  0.0f,  1.0f,  0.0f,  5.0f,  5.0f  // Dreapta-jos
        };
        glGenVertexArrays(1, &m_ceilingVao);
        glGenBuffers(1, &m_ceilingVbo);
        glBindVertexArray(m_ceilingVao);
        glBindBuffer(GL_ARRAY_BUFFER, m_ceilingVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ceilingVertices), ceilingVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), static_cast<void*>(nullptr));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));


        glBindVertexArray(0);
    }


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textures.GetTexture("Ceiling"));


    glBindVertexArray(m_ceilingVao);
    glDrawArrays(GL_TRIANGLES, 0, 6);

}

void SceneRenderer::RenderWalls(const Shader& shader, const float deltaTime)
{
    if (m_wallVao == 0) {
        // Definiți geometria peretelui
        constexpr float wallVertices[] = {
            // Fața
            // Pozitii           // Normale         // Coordonate textură
            -5.0f, -0.5f, -5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Stânga-jos
            -5.0f,  5.0f, -5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 5.0f, // Stânga-sus
             5.0f,  5.0f, -5.0f, 0.0f, 0.0f, -1.0f, 5.0f, 5.0f, // Dreapta-sus
             5.0f,  5.0f, -5.0f, 0.0f, 0.0f, -1.0f, 5.0f, 5.0f, // Dreapta-sus
             5.0f, -0.5f, -5.0f, 0.0f, 0.0f, -1.0f, 5.0f, 0.0f, // Dreapta-jos
            -5.0f, -0.5f, -5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Stânga-jos
            // Spatele
            // Adăugați definițiile pentru spate (inversa fața)
            -5.0f, -0.5f,  5.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Stânga-jos
             5.0f,  5.0f,  5.0f, 0.0f, 0.0f, 1.0f, 5.0f, 5.0f, // Dreapta-sus
            -5.0f,  5.0f,  5.0f, 0.0f, 0.0f, 1.0f, 0.0f, 5.0f, // Stânga-sus
             5.0f, -0.5f,  5.0f, 0.0f, 0.0f, 1.0f, 5.0f, 0.0f, // Dreapta-jos
             5.0f,  5.0f,  5.0f, 0.0f, 0.0f, 1.0f, 5.0f, 5.0f, // Dreapta-sus
            -5.0f, -0.5f,  5.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Stânga-jos
            // Stânga
            // Adăugați definițiile pentru stânga (rotire la stânga față)
            -5.0f, -0.5f,  5.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Stânga-jos
            -5.0f,  5.0f,  5.0f, -1.0f, 0.0f, 0.0f, 5.0f, 0.0f, // Stânga-sus
            -5.0f,  5.0f, -5.0f, -1.0f, 0.0f, 0.0f, 5.0f, 5.0f, // Dreapta-sus
            -5.0f,  5.0f, -5.0f, -1.0f, 0.0f, 0.0f, 5.0f, 5.0f, // Dreapta-sus
            -5.0f, -0.5f, -5.0f, -1.0f, 0.0f, 0.0f, 0.0f, 5.0f, // Dreapta-jos
            -5.0f, -0.5f,  5.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Stânga-jos
            // Dreapta
            // Adăugați definițiile pentru dreapta (rotire la dreapta față)
             5.0f, -0.5f, -5.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Stânga-jos
             5.0f,  5.0f, -5.0f, 1.0f, 0.0f, 0.0f, 5.0f, 0.0f, // Stânga-sus
             5.0f,  5.0f,  5.0f, 1.0f, 0.0f, 0.0f, 5.0f, 5.0f, // Dreapta-sus
             5.0f,  5.0f,  5.0f, 1.0f, 0.0f, 0.0f, 5.0f, 5.0f, // Dreapta-sus
             5.0f, -0.5f,  5.0f, 1.0f, 0.0f, 0.0f, 0.0f, 5.0f, // Dreapta-jos
             5.0f, -0.5f, -5.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f  // Stânga-jos
        };

        glGenVertexArrays(1, &m_wallVao);
        glGenBuffers(1, &m_wallVbo);
        glBindVertexArray(m_wallVao);
        glBindBuffer(GL_ARRAY_BUFFER, m_wallVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(wallVertices), wallVertices, GL_STATIC_DRAW);
        // Specificați atributele vertex-ului
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), static_cast<void*>(nullptr));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
        glBindVertexArray(0);
    }

    // Texturați pereții
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textures.GetTexture("Walls"));

    // Desenați pereții
    glBindVertexArray(m_wallVao);
    glDrawArrays(GL_TRIANGLES, 0, 36); // Numărul total de vertecși pentru toți pereții
    glBindVertexArray(0);
}

void SceneRenderer::RenderAquariumFloor(const Shader& shader, float deltaTime)
{
    if (m_aquariumFloorVao == 0) {
        // set up vertex data (and buffer(s)) and configure vertex attributes
        constexpr float aFloorVertices[] = {
            // positions            // normals         // texcoords
            1.5f, -0.49f, 1.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Dreapta-sus
            -1.5f, -0.49f, 1.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // Stânga-sus
            -1.5f, -0.49f, -1.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // Stânga-jos
            1.5f, -0.49f, 1.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Dreapta-sus
            -1.5f, -0.49f, -1.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // Stânga-jos
            1.5f, -0.49f, -1.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f // Dreapta-jos

        };
        // plane VAO
        glGenVertexArrays(1, &m_aquariumFloorVao);
        glGenBuffers(1, &m_aquariumFloorVbo);
        glBindVertexArray(m_aquariumFloorVao);
        glBindBuffer(GL_ARRAY_BUFFER, m_aquariumFloorVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(aFloorVertices), aFloorVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), static_cast<void*>(nullptr));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
        glBindVertexArray(0);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textures.GetTexture("AFloor"));

    glBindVertexArray(m_aquariumFloorVao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void SceneRenderer::RenderFish(const Shader& shader, float deltaTime)
{
    glm::mat4 fishModel = glm::mat4(1.0);
    fishModel = glm::translate(fishModel, glm::vec3(-1.0f, 0.0f, 0.0f));  // Translate the fish to the left
    fishModel = glm::scale(fishModel, glm::vec3(0.02f));
    fishModel = glm::rotate(fishModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    shader.SetMat4("model", fishModel);
    m_models.at("clownFish").Draw(shader);
}

void SceneRenderer::RenderCoralFish(const Shader& shader, float deltaTime)
{
    glm::mat4 coralFishModel = glm::mat4(1.0);
    coralFishModel = glm::translate(coralFishModel, glm::vec3(1.0f, 0.0f, 0.0f));  
    coralFishModel = glm::scale(coralFishModel, glm::vec3(0.2f)); 
    coralFishModel = glm::rotate(coralFishModel, glm::radians(-80.0f), glm::vec3(1.0f, 0.0f, 0.0f));  

    shader.SetMat4("model", coralFishModel);
    m_models.at("coralFish").Draw(shader);
}

void SceneRenderer::RenderSkull(const Shader& shader, float deltaTime)
{
    glm::mat4 skullModel = glm::mat4(1.0);
    skullModel = glm::translate(skullModel, glm::vec3(-0.5f, -0.49f, 0.0f));
    skullModel = glm::scale(skullModel, glm::vec3(0.025f));
    skullModel = glm::rotate(skullModel, glm::radians(-80.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    shader.SetMat4("model", skullModel);
    m_models.at("skull").Draw(shader);
}

void SceneRenderer::RenderBlueGreenFish(const Shader& shader, float deltaTime)
{
    glm::mat4 bgFish = glm::mat4(1.0);
    bgFish = glm::translate(bgFish, glm::vec3(1.0f, 0.2f, 0.0f)); 
    bgFish = glm::scale(bgFish, glm::vec3(0.15f));
    bgFish = glm::rotate(bgFish, glm::radians(-80.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    shader.SetMat4("model", bgFish);
    m_models.at("blueGreenFish").Draw(shader);
}

void SceneRenderer::RenderRedCoral(const Shader& shader, float deltaTime)
{
    // Prima instanță de coral
    glm::mat4 redCoral1 = glm::mat4(1.0);
    redCoral1 = glm::translate(redCoral1, glm::vec3(0.3f, -0.48f, 0.0f)); 
    redCoral1 = glm::scale(redCoral1, glm::vec3(0.005f)); 
    redCoral1 = glm::rotate(redCoral1, glm::radians(-80.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 
    shader.SetMat4("model", redCoral1);
    m_models.at("redCoral").Draw(shader);

    // A doua instanță de coral
    glm::mat4 redCoral2 = glm::mat4(1.0);
    redCoral2 = glm::translate(redCoral2, glm::vec3(0.35f, -0.48f, 0.0f)); 
    redCoral2 = glm::scale(redCoral2, glm::vec3(0.005f));
    redCoral2 = glm::rotate(redCoral2, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 
    shader.SetMat4("model", redCoral2);
    m_models.at("redCoral").Draw(shader);

}

void SceneRenderer::RenderBlueTangFish(const Shader& shader, float deltaTime)
{
    glm::mat4 btFishModel = glm::mat4(1.0);
    btFishModel = glm::scale(btFishModel, glm::vec3(0.03f));
    btFishModel = glm::rotate(btFishModel, glm::radians(-90.0f), glm::vec3(1.f, 0.f, 0.f));

    constexpr float fishRadius = 36;
    constexpr float fishSpeed = .2f;
    m_fishAngle -= fishSpeed * deltaTime;
    const float x = fishRadius * cos(m_fishAngle);
    const float z = fishRadius * sin(m_fishAngle);

    btFishModel = glm::translate(btFishModel, glm::vec3(x, z, 0));
    btFishModel = glm::rotate(btFishModel, m_fishAngle, glm::vec3(0.f, 0.f, 1.f));

    shader.SetMat4("model", btFishModel);
    m_models.at("blueFish").Draw(shader);
}
void SceneRenderer::RenderCat(const Shader& shader, float deltaTime)
{
    auto catModel = glm::mat4(1.0f);

    constexpr float catRadius = 2.5f; 
    constexpr float catSpeed = 0.1f;
    m_catAngle += catSpeed * deltaTime;

    const float x = catRadius * cos(m_catAngle);
    const float z = catRadius * sin(m_catAngle);
    catModel = glm::translate(catModel, glm::vec3(x, -0.49f, z));

    float angleToCenter = atan2(0.0f - x, 0.0f - z);
    catModel = glm::rotate(catModel, angleToCenter + glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    catModel = glm::rotate(catModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    catModel = glm::scale(catModel, glm::vec3(0.017f));

    shader.SetMat4("model", catModel);
    m_models.at("cat").Draw(shader);
}

void SceneRenderer::RenderCastle(const Shader& shader, float deltaTime)
{
    glm::mat4 castle = scale(glm::mat4(1.0), glm::vec3(.025f));
    castle = rotate(castle, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    castle = translate(castle, glm::vec3(40.0f, 20.f, -20.0f));
    shader.SetMat4("model", castle);
    m_models.at("castle").Draw(shader);

    castle = scale(glm::mat4(1.0), glm::vec3(.05f));
    castle = rotate(castle, glm::radians(-90.0f), glm::vec3(1.0f, 0.f, 0.0f));
    castle = rotate(castle, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    castle = translate(castle, glm::vec3(20.0f, 15.f, -10.0f));
    shader.SetMat4("model", castle);
    m_models.at("castle").Draw(shader);
}

void SceneRenderer::RenderDaffodils(const Shader& shader, float deltaTime)
{
    for (int i = 0; i < 5; ++i) {
        glm::mat4 daffodil = glm::mat4(1.0);
        float x = -1.0f + i * 0.2f; 
        float y = -0.48f; 
        float z = -1.3f; 
        daffodil = glm::translate(daffodil, glm::vec3(x, y, z));
        daffodil = glm::scale(daffodil, glm::vec3(0.05f));
        daffodil = glm::rotate(daffodil, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        shader.SetMat4("model", daffodil);
        m_models.at("daffodilModel").Draw(shader);
    }
}

