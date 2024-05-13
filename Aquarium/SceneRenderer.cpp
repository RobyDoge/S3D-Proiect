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

    m_fishModel= Model(m_projectPath + "\\Models\\Fish\\12265_Fish_v1_L2.obj", false, false);
    m_coralFishModel = Model(m_projectPath + "\\Models\\Coral_Beauty_Angelfish\\13009_Coral_Beauty_Angelfish_v1_l3.obj", false, false);
  
}

void SceneRenderer::Init()
{
    m_renderers.emplace_back([this](const Shader& shader, const float deltaTime) {RenderCeiling(shader, deltaTime); });
    m_renderers.emplace_back([this](const Shader& shader, const float deltaTime) { RenderFloor(shader, deltaTime); });
    m_renderers.emplace_back([this](const Shader& shader, const float deltaTime) {RenderAquariumFloor(shader, deltaTime); });

    m_renderers.emplace_back([this](const Shader& shader, const float deltaTime) { RenderWalls(shader, deltaTime); });
    m_renderers.emplace_back([this](const Shader& shader, const float deltaTime) {RenderFish(shader, deltaTime); });
    m_renderers.emplace_back([this](const Shader& shader, const float deltaTime) {RenderCoralFish(shader, deltaTime); });
    m_renderers.emplace_back([this](const Shader& shader, const float deltaTime) {RenderStarfish(shader, deltaTime); });
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
    
    if (m_fishVao == 0) {
        // Define vertex data for a cube
        constexpr float aquariumVertices[] = {
            // Poziții scalate cu factorul scale    // Normale scalate        
            -1.0f * 1, -1.0f * 1, -1.0f * 1,  0.0f, 0.0f, -1.0f,
             1.0f * 1, -1.0f * 1, -1.0f * 1,  0.0f, 0.0f, -1.0f,
             1.0f * 1,  1.0f * 1, -1.0f * 1,  0.0f, 0.0f, -1.0f,
             1.0f * 1,  1.0f * 1, -1.0f * 1,  0.0f, 0.0f, -1.0f,
            -1.0f * 1,  1.0f * 1, -1.0f * 1,  0.0f, 0.0f, -1.0f,
            -1.0f * 1, -1.0f * 1, -1.0f * 1,  0.0f, 0.0f, -1.0f,

            -1.0f * 1, -1.0f * 1,  1.0f * 1,  0.0f, 0.0f,  1.0f,
             1.0f * 1, -1.0f * 1,  1.0f * 1,  0.0f, 0.0f,  1.0f,
             1.0f * 1,  1.0f * 1,  1.0f * 1,  0.0f, 0.0f,  1.0f,
             1.0f * 1,  1.0f * 1,  1.0f * 1,  0.0f, 0.0f,  1.0f,
            -1.0f * 1,  1.0f * 1,  1.0f * 1,  0.0f, 0.0f,  1.0f,
            -1.0f * 1, -1.0f * 1,  1.0f * 1,  0.0f, 0.0f,  1.0f,

            -1.0f * 1,  1.0f * 1,  1.0f * 1, -1.0f, 0.0f, 0.0f,
            -1.0f * 1,  1.0f * 1, -1.0f * 1, -1.0f, 0.0f, 0.0f,
            -1.0f * 1, -1.0f * 1, -1.0f * 1, -1.0f, 0.0f, 0.0f,
            -1.0f * 1, -1.0f * 1, -1.0f * 1, -1.0f, 0.0f, 0.0f,
            -1.0f * 1, -1.0f * 1,  1.0f * 1, -1.0f, 0.0f, 0.0f,
            -1.0f * 1,  1.0f * 1,  1.0f * 1, -1.0f, 0.0f, 0.0f,

             1.0f * 1,  1.0f * 1,  1.0f * 1,  1.0f, 0.0f, 0.0f,
             1.0f * 1,  1.0f * 1, -1.0f * 1,  1.0f, 0.0f, 0.0f,
             1.0f * 1, -1.0f * 1, -1.0f * 1,  1.0f, 0.0f, 0.0f,
             1.0f * 1, -1.0f * 1, -1.0f * 1,  1.0f, 0.0f, 0.0f,
             1.0f * 1, -1.0f * 1,  1.0f * 1,  1.0f, 0.0f, 0.0f,
             1.0f * 1,  1.0f * 1,  1.0f * 1,  1.0f, 0.0f, 0.0f,

            -1.0f * 1, -1.0f * 1, -1.0f * 1,  0.0f, -1.0f, 0.0f,
             1.0f * 1, -1.0f * 1, -1.0f * 1,  0.0f, -1.0f, 0.0f,
             1.0f * 1, -1.0f * 1,  1.0f * 1,  0.0f, -1.0f, 0.0f,
             1.0f * 1, -1.0f * 1,  1.0f * 1,  0.0f, -1.0f, 0.0f,
            -1.0f * 1, -1.0f * 1,  1.0f * 1,  0.0f, -1.0f, 0.0f,
            -1.0f * 1, -1.0f * 1, -1.0f * 1,  0.0f, -1.0f, 0.0f,

            -1.0f * 1,  1.0f * 1, -1.0f * 1,  0.0f,  1.0f, 0.0f,
             1.0f * 1,  1.0f * 1, -1.0f * 1,  0.0f,  1.0f, 0.0f,
             1.0f * 1,  1.0f * 1,  1.0f * 1,  0.0f,  1.0f, 0.0f,
             1.0f * 1,  1.0f * 1,  1.0f * 1,  0.0f,  1.0f, 0.0f,
            -1.0f * 1,  1.0f * 1,  1.0f * 1,  0.0f,  1.0f, 0.0f,
            -1.0f * 1,  1.0f * 1, -1.0f * 1,  0.0f,  1.0f, 0.0f,
        };
        glGenVertexArrays(1, &m_fishVao);
        glGenBuffers(1, &m_fishVbo);
        glBindVertexArray(m_fishVao);
        glBindBuffer(GL_ARRAY_BUFFER, m_fishVbo);
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
    glm::mat4 fishModel = glm::scale(glm::mat4(1.0), glm::vec3(0.01f));
    fishModel = glm::rotate(fishModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        
    shader.SetMat4("model", fishModel);
    m_fishModel.Draw(shader);
}

void SceneRenderer::RenderCoralFish(const Shader& shader, float deltaTime)
{
    // Declare coralFishModel
    glm::mat4 coralFishModel = glm::scale(glm::mat4(1.0), glm::vec3(0.05f));
    coralFishModel = glm::rotate(coralFishModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    if (m_coralFishVao == 0) {
        // Define vertex data for a cube
        constexpr float aquariumVertices[] = {
            // Poziții scalate cu factorul scale    // Normale scalate        

            // Front face
            -1.0f - 5.0f, -1.0f + 5.0f, -1.0f,  0.0f, 0.0f, -1.0f,
            1.0f - 5.0f, -1.0f + 5.0f, -1.0f,  0.0f, 0.0f, -1.0f,
            1.0f - 5.0f,  1.0f + 5.0f, -1.0f,  0.0f, 0.0f, -1.0f,
            1.0f - 5.0f,  1.0f + 5.0f, -1.0f,  0.0f, 0.0f, -1.0f,
            -1.0f - 5.0f,  1.0f + 5.0f, -1.0f,  0.0f, 0.0f, -1.0f,
            -1.0f - 5.0f, -1.0f + 5.0f, -1.0f,  0.0f, 0.0f, -1.0f,

            // Back face
            -1.0f - 5.0f, -1.0f + 5.0f,  1.0f,  0.0f, 0.0f,  1.0f,
            1.0f - 5.0f, -1.0f + 5.0f,  1.0f,  0.0f, 0.0f,  1.0f,
            1.0f - 5.0f,  1.0f + 5.0f,  1.0f,  0.0f, 0.0f,  1.0f,
            1.0f - 5.0f,  1.0f + 5.0f,  1.0f,  0.0f, 0.0f,  1.0f,
            -1.0f - 5.0f,  1.0f + 5.0f,  1.0f,  0.0f, 0.0f,  1.0f,
            -1.0f - 5.0f, -1.0f + 5.0f,  1.0f,  0.0f, 0.0f,  1.0f,

            // Left face
            -1.0f - 5.0f,  1.0f + 5.0f,  1.0f, -1.0f, 0.0f, 0.0f,
            -1.0f - 5.0f,  1.0f + 5.0f, -1.0f, -1.0f, 0.0f, 0.0f,
            -1.0f - 5.0f, -1.0f + 5.0f, -1.0f, -1.0f, 0.0f, 0.0f,
            -1.0f - 5.0f, -1.0f + 5.0f, -1.0f, -1.0f, 0.0f, 0.0f,
            -1.0f - 5.0f, -1.0f + 5.0f,  1.0f, -1.0f, 0.0f, 0.0f,
            -1.0f - 5.0f,  1.0f + 5.0f,  1.0f, -1.0f, 0.0f, 0.0f,

            // Right face
            1.0f - 5.0f,  1.0f + 5.0f,  1.0f,  1.0f, 0.0f, 0.0f,
            1.0f - 5.0f,  1.0f + 5.0f, -1.0f,  1.0f, 0.0f, 0.0f,
            1.0f - 5.0f, -1.0f + 5.0f, -1.0f,  1.0f, 0.0f, 0.0f,
            1.0f - 5.0f, -1.0f + 5.0f, -1.0f,  1.0f, 0.0f, 0.0f,
            1.0f - 5.0f, -1.0f + 5.0f,  1.0f,  1.0f, 0.0f, 0.0f,
            1.0f - 5.0f,  1.0f + 5.0f,  1.0f,  1.0f, 0.0f, 0.0f,

            // Bottom face
            -1.0f - 5.0f, -1.0f + 5.0f, -1.0f,  0.0f, -1.0f, 0.0f,
            1.0f - 5.0f, -1.0f + 5.0f, -1.0f,  0.0f, -1.0f, 0.0f,
            1.0f - 5.0f, -1.0f + 5.0f,  1.0f,  0.0f, -1.0f, 0.0f,
            1.0f - 5.0f, -1.0f + 5.0f,  1.0f,  0.0f, -1.0f, 0.0f,
            -1.0f - 5.0f, -1.0f + 5.0f,  1.0f,  0.0f, -1.0f, 0.0f,
            -1.0f - 5.0f, -1.0f + 5.0f, -1.0f,  0.0f, -1.0f, 0.0f,

            // Top face
            -1.0f - 5.0f,  1.0f + 5.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            1.0f - 5.0f,  1.0f + 5.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            1.0f - 5.0f,  1.0f + 5.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            1.0f - 5.0f,  1.0f + 5.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -1.0f - 5.0f,  1.0f + 5.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -1.0f - 5.0f,  1.0f + 5.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        };

        glGenVertexArrays(1, &m_coralFishVao);
        glGenBuffers(1, &m_coralFishVbo);
        glBindVertexArray(m_coralFishVao);
        glBindBuffer(GL_ARRAY_BUFFER, m_coralFishVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(aquariumVertices), aquariumVertices, GL_STATIC_DRAW);

        // Atributul pentru poziție
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void*>(nullptr));
        glEnableVertexAttribArray(0);
        // Atributul pentru normală
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    shader.SetMat4("model", coralFishModel);
    m_coralFishModel.Draw(shader);
}

void SceneRenderer::RenderStarfish(const Shader& shader, float deltaTime)
{
    // Declare starfish model
    glm::mat4 starfish = glm::scale(glm::mat4(1.0), glm::vec3(0.1f));
    starfish = glm::rotate(starfish, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    if (m_starfishVao == 0) {
        // Define vertex data for a cube
        constexpr float aquariumVertices[] = {
            // Poziții scalate cu factorul scale    // Normale scalate        

            // Front face
            -1.0f - 5.0f, -1.0f + 5.0f, -1.0f,  0.0f, 0.0f, -1.0f,
            1.0f - 5.0f, -1.0f + 5.0f, -1.0f,  0.0f, 0.0f, -1.0f,
            1.0f - 5.0f,  1.0f + 5.0f, -1.0f,  0.0f, 0.0f, -1.0f,
            1.0f - 5.0f,  1.0f + 5.0f, -1.0f,  0.0f, 0.0f, -1.0f,
            -1.0f - 5.0f,  1.0f + 5.0f, -1.0f,  0.0f, 0.0f, -1.0f,
            -1.0f - 5.0f, -1.0f + 5.0f, -1.0f,  0.0f, 0.0f, -1.0f,

            // Back face
            -1.0f - 5.0f, -1.0f + 5.0f,  1.0f,  0.0f, 0.0f,  1.0f,
            1.0f - 5.0f, -1.0f + 5.0f,  1.0f,  0.0f, 0.0f,  1.0f,
            1.0f - 5.0f,  1.0f + 5.0f,  1.0f,  0.0f, 0.0f,  1.0f,
            1.0f - 5.0f,  1.0f + 5.0f,  1.0f,  0.0f, 0.0f,  1.0f,
            -1.0f - 5.0f,  1.0f + 5.0f,  1.0f,  0.0f, 0.0f,  1.0f,
            -1.0f - 5.0f, -1.0f + 5.0f,  1.0f,  0.0f, 0.0f,  1.0f,

            // Left face
            -1.0f - 5.0f,  1.0f + 5.0f,  1.0f, -1.0f, 0.0f, 0.0f,
            -1.0f - 5.0f,  1.0f + 5.0f, -1.0f, -1.0f, 0.0f, 0.0f,
            -1.0f - 5.0f, -1.0f + 5.0f, -1.0f, -1.0f, 0.0f, 0.0f,
            -1.0f - 5.0f, -1.0f + 5.0f, -1.0f, -1.0f, 0.0f, 0.0f,
            -1.0f - 5.0f, -1.0f + 5.0f,  1.0f, -1.0f, 0.0f, 0.0f,
            -1.0f - 5.0f,  1.0f + 5.0f,  1.0f, -1.0f, 0.0f, 0.0f,

            // Right face
            1.0f - 5.0f,  1.0f + 5.0f,  1.0f,  1.0f, 0.0f, 0.0f,
            1.0f - 5.0f,  1.0f + 5.0f, -1.0f,  1.0f, 0.0f, 0.0f,
            1.0f - 5.0f, -1.0f + 5.0f, -1.0f,  1.0f, 0.0f, 0.0f,
            1.0f - 5.0f, -1.0f + 5.0f, -1.0f,  1.0f, 0.0f, 0.0f,
            1.0f - 5.0f, -1.0f + 5.0f,  1.0f,  1.0f, 0.0f, 0.0f,
            1.0f - 5.0f,  1.0f + 5.0f,  1.0f,  1.0f, 0.0f, 0.0f,

            // Bottom face
            -1.0f - 5.0f, -1.0f + 5.0f, -1.0f,  0.0f, -1.0f, 0.0f,
            1.0f - 5.0f, -1.0f + 5.0f, -1.0f,  0.0f, -1.0f, 0.0f,
            1.0f - 5.0f, -1.0f + 5.0f,  1.0f,  0.0f, -1.0f, 0.0f,
            1.0f - 5.0f, -1.0f + 5.0f,  1.0f,  0.0f, -1.0f, 0.0f,
            -1.0f - 5.0f, -1.0f + 5.0f,  1.0f,  0.0f, -1.0f, 0.0f,
            -1.0f - 5.0f, -1.0f + 5.0f, -1.0f,  0.0f, -1.0f, 0.0f,

            // Top face
            -1.0f - 5.0f,  1.0f + 5.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            1.0f - 5.0f,  1.0f + 5.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            1.0f - 5.0f,  1.0f + 5.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            1.0f - 5.0f,  1.0f + 5.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -1.0f - 5.0f,  1.0f + 5.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -1.0f - 5.0f,  1.0f + 5.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        };



        glGenVertexArrays(1, &m_starfishVao);
        glGenBuffers(1, &m_starfishVbo);
        glBindVertexArray(m_starfishVao);
        glBindBuffer(GL_ARRAY_BUFFER, m_starfishVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(aquariumVertices), aquariumVertices, GL_STATIC_DRAW);

        // Atributul pentru poziție
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void*>(nullptr));
        glEnableVertexAttribArray(0);
        // Atributul pentru normală
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    shader.SetMat4("model", starfish);
    m_coralFishModel.Draw(shader);
}
