#include "Textures.h"
#include <format>
#include <stb_image.h>
#include <glew.h>

Textures::Textures(const string& projectPath)
{
    std::string strExePath;
    if (const size_t last_slash_idx = projectPath.rfind('\\'); 
        std::string::npos != last_slash_idx) {
        strExePath = projectPath.substr(0, last_slash_idx);
    }
    m_texturesPath = strExePath + R"(\..\..\Images\)";
}

void Textures::AddTexture(const string& textureAliasName, const string& imageName)
{
    unsigned int textureId = -1;

    auto texturePath{ m_texturesPath + imageName };

    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
    if(!data) throw std::runtime_error(std::format("Failed to load texture: {}", texturePath));
	if (data) {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    stbi_image_free(data);
    m_textures.insert({textureAliasName,textureId});
}

unsigned Textures::GetTexture(const string& textureName) const
{
	return m_textures.at(textureName);
}
