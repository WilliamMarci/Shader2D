#pragma once

#include <glm/glm.hpp>
#include <string>

class Shader {
  public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    void UploadUniformInt(const std::string& name, int value);
    void UploadUniformFloat(const std::string& name, float value);
    void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
    void UploadUniformMat4(const std::string& name, const glm::mat4& value);

  private:
    std::string ReadFile(const std::string& filepath);
    unsigned int CompileShader(unsigned int type, const std::string& source);

  private:
    unsigned int m_RendererID;
};