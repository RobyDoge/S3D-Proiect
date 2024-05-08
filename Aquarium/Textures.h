#pragma once
#include <string>
#include <unordered_map>
#define STB_IMAGE_IMPLEMENTATION


using std::string;
class Textures
{
public:
	struct TextureData
	{
		unsigned int id;
		string type;
	};

public:
	Textures() = default;
	Textures(const string& projectPath);
	~Textures() = default;
	void AddTexture(const string& textureAliasName, const string& imageName);

	TextureData GetTexture(const string& textureName) const;
	std::unordered_map<string, TextureData> GetTextures();

private:
	string m_texturesPath;
	//textureName + texture itself
	std::unordered_map<string, TextureData> m_textures;
	
};
