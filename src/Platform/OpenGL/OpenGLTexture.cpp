#include "OpenGLTexture.h"
#include <iostream>
#include "stb_image.h"

#define USE_OPENGL_45_DSA 1

namespace Engine {

OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height) : m_Width(width), m_Height(height) {
    m_InternalFormat = GL_RGBA8;
    m_DataFormat = GL_RGBA;
#if USE_OPENGL_45_DSA
    // --- OpenGL 4.5 DSA ---
    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
    glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);
    glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
#else
    // --- OpenGL 3.3 ---
    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
#endif
}

OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : m_Path(path) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1); // OpenGL Left Bottom Origin
    stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    if (!data) {
        std::cerr << "[PLATFORM] Failed to load image: " << path << std::endl;
        return;
    }

    m_Width = width;
    m_Height = height;
    std::cout << "[PLATFORM] Loaded image: " << path << " (" << m_Width << "x" << m_Height << ", " << channels << " channels)" << std::endl;
    if (channels == 4) {
        m_InternalFormat = GL_RGBA8;
        m_DataFormat = GL_RGBA;
    } else if (channels == 3) {
        m_InternalFormat = GL_RGB8;
        m_DataFormat = GL_RGB;
    } else {
        std::cerr << "[PLATFORM] Unsupported image format: " << path << std::endl;
        stbi_image_free(data);
        return;
    }

#if USE_OPENGL_45_DSA
    // --- OpenGL 4.5 DSA ---
    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
    glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);
    glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
#else
    // --- OpenGL 3.3 ---
    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, width, height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
#endif
    stbi_image_free(data);
}

OpenGLTexture2D::~OpenGLTexture2D() { glDeleteTextures(1, &m_RendererID); }

void OpenGLTexture2D::SetData(void* data, uint32_t size) {
    uint32_t bpp = (m_DataFormat == GL_RGBA) ? 4 : 3;
    if (size != m_Width * m_Height * bpp) {
        std::cerr << "Data size does not match texture size!" << std::endl;
        return;
    }

#if USE_OPENGL_45_DSA
    glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
#else
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
#endif
}

void OpenGLTexture2D::Bind(uint32_t slot) const {
#if USE_OPENGL_45_DSA
    glBindTextureUnit(slot, m_RendererID);
#else
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
#endif
}

}