#include "Shader.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLShader.h" //Platform specific

#include <iostream>

std::shared_ptr<Shader> Shader::Create(const std::string& vertexPath, const std::string& fragmentPath) {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:    
            std::cerr << "RendererAPI::None is currently not supported!" << std::endl;
            return nullptr;
        case RendererAPI::API::OpenGL:  
            return std::make_shared<OpenGLShader>(vertexPath, fragmentPath);
    }

    std::cerr << "Unknown RendererAPI!" << std::endl;
    return nullptr;
}
