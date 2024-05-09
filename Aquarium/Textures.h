#pragma once
#include <string>
#include <unordered_map>
#define STB_IMAGE_IMPLEMENTATION


using std::string;
class Textures
{
public:

public:
	Textures() = default;
	Textures(const string& projectPath);
	~Textures() = default;
	void Add(const Textures& other);

	void AddTexture(const string& textureAliasName, const string& imageName);/*
	void AddTexture(const TextureData& texture);
	void AddTexture(const string& textureAliasName, const string& imageName, const string& typeName);*/
	
	unsigned int GetTexture(const string& textureName) const;

private:
	string m_texturesPath;
	//textureName + texture itself
	std::unordered_map<string, unsigned int> m_textures;
	
};
