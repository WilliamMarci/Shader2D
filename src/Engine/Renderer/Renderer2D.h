#pragma once

#include "Engine/Renderer/Camera.h"
#include "Engine/Renderer/Texture.h" // 引入 Texture
#include "Engine/Renderer/Shader.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace Engine {

struct Vertex {
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TexCoord; // 新增：UV坐标
    float TexIndex;     // 新增：纹理槽索引 (用 float 传给 shader)
};

struct RendererStats {
    uint32_t DrawCalls = 0;
    uint32_t QuadCount = 0;
};

class Renderer2D {
public:
    static void Init();
    static void Shutdown();

    static void BeginScene(const Camera& camera, Shader& shader);
    static void EndScene();
    
    // Colored Quad
    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
    static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
    
    // Textured Quad
    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
    static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
    
    static RendererStats& GetStats();
    static void ResetStats();
    
    private:
    static void Flush();
    static void BeginBatch();
    static void EndBatch();
    static bool IsOnScreen(const glm::vec2& pos, const glm::vec2& size);
};

}