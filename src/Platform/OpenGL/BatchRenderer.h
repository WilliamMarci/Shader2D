#pragma once

#include "Engine/Renderer/Camera.h"
#include "Platform/OpenGL/Shader.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

struct Vertex {
    glm::vec3 Position;
    glm::vec4 Color;
    // [TODO]: Future features
    // vlm::vec2 TexCoord;
    // float TexIndex;
};

struct RendererStats {
    uint32_t DrawCalls = 0;
    uint32_t QuadCount = 0;
};

class BatchRenderer {
  public:
    static void Init();
    static void Shutdown();
    // --- Sence ---
    static void BeginScene(const Camera& camera, Shader& shader);
    static void EndScene();
    // --- Draw ---
    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
    
    // --- Stat Info ---
    static RendererStats GetStats();
    static void ResetStats();
    
    private:
    static void Flush();
    static void BeginBatch();
    static void EndBatch();
    static bool IsOnScreen(const glm::vec2& pos, const glm::vec2& size);
};