#include "SceneRenderer.h"
#include <glew.h>
#include <memory>
#include <string>


SceneRenderer::SceneRenderer(const string& projectPath) : m_textures{ projectPath }
{
    m_textures.AddTexture("Floor", "Floor.jpg");
    m_textures.AddTexture("Ceiling", "Ceiling.jpg");
    m_textures.AddTexture("Walls", "Walls.jpg");
}

void SceneRenderer::Init()
{
    m_renderers.emplace_back([this](const Shader& shader, const float deltaTime) {RenderCeiling(shader,deltaTime); });
    m_renderers.emplace_back([this](const Shader& shader, const float deltaTime) { RenderFloor(shader, deltaTime); });
    m_renderers.emplace_back([this](const Shader& shader, const float deltaTime) { RenderWalls(shader, deltaTime); });
}

void SceneRenderer::Render(const Shader& shader, float deltaTime) const
{
	constexpr glm::mat4 model{1};
    shader.SetMat4("model", model);
	for (const auto& render : m_renderers)
	{
		render(shader,deltaTime);
	}
    RenderAquarium(shader, deltaTime);
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

void SceneRenderer::RenderAquarium(const Shader& shader, float deltaTime) const
{
    // Definirea geometriei acvariului
    constexpr float aquariumVertices[] = {
        // Pozitii           // Normale         // Coordonate textură
        // Față
        -2.0f, -0.2f, -2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Stânga-jos
        -2.0f,  2.0f, -2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, // Stânga-sus
         2.0f,  2.0f, -2.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // Dreapta-sus
         2.0f,  2.0f, -2.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // Dreapta-sus
         2.0f, -0.2f, -2.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // Dreapta-jos
        -2.0f, -0.2f, -2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Stânga-jos
        // Spate
        // Adăugați definițiile pentru spate (inversa fața)
        -2.0f, -0.2f,  2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Stânga-jos
         2.0f,  2.0f,  2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Dreapta-sus
        -2.0f,  2.0f,  2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // Stânga-sus
         2.0f, -0.2f,  2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // Dreapta-jos
         2.0f,  2.0f,  2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Dreapta-sus
        -2.0f, -0.2f,  2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Stânga-jos
        // Stânga
        // Adăugați definițiile pentru stânga (rotire la stânga față)
        -2.0f, -0.2f,  2.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Stânga-jos
        -2.0f,  2.0f,  2.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Stânga-sus
        -2.0f,  2.0f, -2.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Dreapta-sus
        -2.0f,  2.0f, -2.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Dreapta-sus
        -2.0f, -0.2f, -2.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Dreapta-jos
        -2.0f, -0.2f,  2.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Stânga-jos
        // Dreapta
        // Adăugați definițiile pentru dreapta (rotire la dreapta față)
         2.0f, -0.2f, -2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Stânga-jos
         2.0f,  2.0f, -2.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Stânga-sus
         2.0f,  2.0f,  2.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Dreapta-sus
         2.0f,  2.0f,  2.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Dreapta-sus
         2.0f, -0.2f,  2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Dreapta-jos
         2.0f, -0.2f, -2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f  // Stânga-jos
    };

    unsigned int aquariumVao = 0;
    unsigned int aquariumVbo = 0;

    if (aquariumVao == 0) {
        glGenVertexArrays(1, &aquariumVao);
        glGenBuffers(1, &aquariumVbo);
        glBindVertexArray(aquariumVao);
        glBindBuffer(GL_ARRAY_BUFFER, aquariumVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(aquariumVertices), aquariumVertices, GL_STATIC_DRAW);
        // Specificați atributele vertex-ului
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), static_cast<void*>(nullptr));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
        glBindVertexArray(0);
    }

    //TEXTURA SAU O CULOARE GRI-ALBASTRIE CU TRANSPARENTA MARE CARE SA IMITE STICLA???????

    //glm::vec4 aquariumColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f); // Alb-gri cu transparență
    //shader.SetVec4("aquariumColor", aquariumColor);

    //// Texturați acvariul
    //glActiveTexture(GL_TEXTURE0);
    //// Presupunând că aveți o textură pentru acvariu
    //glBindTexture(GL_TEXTURE_2D, m_textures.GetTexture("Aquarium"));

    // Desenați acvariul
    glBindVertexArray(aquariumVao);
    glDrawArrays(GL_TRIANGLES, 0, 36); // Numărul total de vertecși pentru toate fețele acvariului
    glBindVertexArray(0);
}
