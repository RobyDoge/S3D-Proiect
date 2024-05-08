﻿#include "SceneRenderer.h"
#include <glew.h>
#include <memory>
#include <string>


#define GLM_FORCE_CTOR_INIT 
#include <GLM.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>


SceneRenderer::SceneRenderer(const string& projectPath) : m_textures{ projectPath }
{
    m_textures.AddTexture("Floor", "Floor.jpg");
    m_textures.AddTexture("Ceiling", "Ceiling.jpg");
    m_textures.AddTexture("Walls", "Walls.jpg");
    m_textures.AddTexture("aqua", "aqua.jpg");
}

void SceneRenderer::Init()
{
    m_renderers.emplace_back([this](const Shader& shader, const float deltaTime) {RenderCeiling(shader, deltaTime); });
    m_renderers.emplace_back([this](const Shader& shader, const float deltaTime) { RenderFloor(shader, deltaTime); });
    m_renderers.emplace_back([this](const Shader& shader, const float deltaTime) { RenderWalls(shader, deltaTime); });
    m_renderers.emplace_back([this](const Shader& shader, const float deltaTime) {RenderAquarium(shader, deltaTime); });
}

void SceneRenderer::Render(const Shader& shader, float deltaTime) const
{
    constexpr glm::mat4 model{ 1 };
    shader.SetMat4("model", model);
    for (const auto& render : m_renderers)
    {
        render(shader, deltaTime);
    }
}

void SceneRenderer::RenderFloor(const Shader& shader, const float deltaTime)
{
    if (m_planeVao == 0) {
        // set up vertex data (and buffer(s)) and configure vertex attributes
        constexpr float planeVertices[] = {
            // positions            // normals         // texcoords
            25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
            -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
            -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

            25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
            -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
            25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
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
    glBindTexture(GL_TEXTURE_2D, m_textures.GetTexture("Floor").id);

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
    glBindTexture(GL_TEXTURE_2D, m_textures.GetTexture("Ceiling").id);


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
    glBindTexture(GL_TEXTURE_2D, m_textures.GetTexture("Walls").id);

    // Desenați pereții
    glBindVertexArray(m_wallVao);
    glDrawArrays(GL_TRIANGLES, 0, 36); // Numărul total de vertecși pentru toți pereții
    glBindVertexArray(0);
}

void SceneRenderer::RenderAquarium(const Shader& shader, const float deltaTime)
{
    constexpr float scale_x = 0.5f; // Scalarea pe axa x
    constexpr float scale_y = 0.3f; // Scalarea pe axa y
    constexpr float scale_z = 0.7f; // Scalarea pe axa z

    if (m_aquariumVao == 0) {
        // Define vertex data for a cube
        constexpr float aquariumVertices[] = {
            // Poziții scalate cu factorul scale    // Normale scalate         // Coordonate textură
            -1.0f * scale_x, -1.0f * scale_y, -1.0f * scale_z,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
             1.0f * scale_x, -1.0f * scale_y, -1.0f * scale_z,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f,
             1.0f * scale_x,  1.0f * scale_y, -1.0f * scale_z,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
             1.0f * scale_x,  1.0f * scale_y, -1.0f * scale_z,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
            -1.0f * scale_x,  1.0f * scale_y, -1.0f * scale_z,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f,
            -1.0f * scale_x, -1.0f * scale_y, -1.0f * scale_z,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,

            -1.0f * scale_x, -1.0f * scale_y,  1.0f * scale_z,  0.0f, 0.0f,  1.0f,  0.0f, 0.0f,
             1.0f * scale_x, -1.0f * scale_y,  1.0f * scale_z,  0.0f, 0.0f,  1.0f,  1.0f, 0.0f,
             1.0f * scale_x,  1.0f * scale_y,  1.0f * scale_z,  0.0f, 0.0f,  1.0f,  1.0f, 1.0f,
             1.0f * scale_x,  1.0f * scale_y,  1.0f * scale_z,  0.0f, 0.0f,  1.0f,  1.0f, 1.0f,
            -1.0f * scale_x,  1.0f * scale_y,  1.0f * scale_z,  0.0f, 0.0f,  1.0f,  0.0f, 1.0f,
            -1.0f * scale_x, -1.0f * scale_y,  1.0f * scale_z,  0.0f, 0.0f,  1.0f,  0.0f, 0.0f,

            -1.0f * scale_x,  1.0f * scale_y,  1.0f * scale_z, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
            -1.0f * scale_x,  1.0f * scale_y, -1.0f * scale_z, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
            -1.0f * scale_x, -1.0f * scale_y, -1.0f * scale_z, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
            -1.0f * scale_x, -1.0f * scale_y, -1.0f * scale_z, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
            -1.0f * scale_x, -1.0f * scale_y,  1.0f * scale_z, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
            -1.0f * scale_x,  1.0f * scale_y,  1.0f * scale_z, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,

             1.0f * scale_x,  1.0f * scale_y,  1.0f * scale_z,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
             1.0f * scale_x,  1.0f * scale_y, -1.0f * scale_z,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
             1.0f * scale_x, -1.0f * scale_y, -1.0f * scale_z,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
             1.0f * scale_x, -1.0f * scale_y, -1.0f * scale_z,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
             1.0f * scale_x, -1.0f * scale_y,  1.0f * scale_z,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
             1.0f * scale_x,  1.0f * scale_y,  1.0f * scale_z,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,

            -1.0f * scale_x, -1.0f * scale_y, -1.0f * scale_z,  0.0f, -1.0f, 0.0f,  0.0f, 1.0f,
             1.0f * scale_x, -1.0f * scale_y, -1.0f * scale_z,  0.0f, -1.0f, 0.0f,  1.0f, 1.0f,
             1.0f * scale_x, -1.0f * scale_y,  1.0f * scale_z,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
             1.0f * scale_x, -1.0f * scale_y,  1.0f * scale_z,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
            -1.0f * scale_x, -1.0f * scale_y,  1.0f * scale_z,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
            -1.0f * scale_x, -1.0f * scale_y, -1.0f * scale_z,  0.0f, -1.0f, 0.0f,  0.0f, 1.0f,

            -1.0f * scale_x,  1.0f * scale_y, -1.0f * scale_z,  0.0f,  1.0f, 0.0f,  0.0f, 1.0f,
             1.0f * scale_x,  1.0f * scale_y, -1.0f * scale_z,  0.0f,  1.0f, 0.0f,  1.0f, 1.0f,
             1.0f * scale_x,  1.0f * scale_y,  1.0f * scale_z,  0.0f,  1.0f, 0.0f,  1.0f, 0.0f,
             1.0f * scale_x,  1.0f * scale_y,  1.0f * scale_z,  0.0f,  1.0f, 0.0f,  1.0f, 0.0f,
            -1.0f * scale_x,  1.0f * scale_y,  1.0f * scale_z,  0.0f,  1.0f, 0.0f,  0.0f, 0.0f,
            -1.0f * scale_x,  1.0f * scale_y, -1.0f * scale_z,  0.0f,  1.0f, 0.0f,  0.0f, 1.0f
        };


        // Generarea VAO și VBO
        glGenVertexArrays(1, &m_aquariumVao);
        glGenBuffers(1, &m_aquariumVbo);
        glBindVertexArray(m_aquariumVao);
        glBindBuffer(GL_ARRAY_BUFFER, m_aquariumVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(aquariumVertices), aquariumVertices, GL_STATIC_DRAW);

        // Atributul pentru poziție
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Atributul pentru normală
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // Atributul pentru coordonate textură
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    // Bind textura
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textures.GetTexture("aqua").id);

    // Renderea cubului
    glBindVertexArray(m_aquariumVao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

}

