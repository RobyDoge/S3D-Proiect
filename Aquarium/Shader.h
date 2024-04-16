#pragma once
#include <string>
#include <GLM.hpp>


class Shader
{
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    void Use() const;
    unsigned int GetId() const;

    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVec3(const std::string& name, const glm::vec3& value) const;
    void SetVec3(const std::string& name, float x, float y, float z) const;
    void SetMat4(const std::string& name, const glm::mat4& mat) const;

private:
    void Init(const char* vertexPath, const char* fragmentPath);
    static void CheckCompileErrors(unsigned int shader, const std::string& type);

private:
    unsigned int m_id;
};
