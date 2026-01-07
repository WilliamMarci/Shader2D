#include "Engine/Renderer/Texture.h"
#include "Engine/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Engine {

std::shared_ptr<Texture2D> Texture2D::Create(uint32_t width, uint32_t height) {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:    return nullptr;
        case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLTexture2D>(width, height);
    }
    return nullptr;
}

std::shared_ptr<Texture2D> Texture2D::Create(const std::string& path) {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:    return nullptr;
        case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLTexture2D>(path);
    }
    return nullptr;
}

}