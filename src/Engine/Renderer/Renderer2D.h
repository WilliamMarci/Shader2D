#pragma once

#include "Engine/Renderer/Camera.h"
#include "Engine/Renderer/Shader.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

struct Vertex {
    glm::vec3 Position;
    glm::vec4 Color;
    // [TODO]: Future features
    // glm::vec2 TexCoord;
    // float TexIndex;
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

    // Primitives
    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
    static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
    
    // Check if object is in camera view (Culling)
    static bool IsOnScreen(const glm::vec2& pos, const glm::vec2& size);

    // Stats
    static RendererStats& GetStats();
    static void ResetStats();

private:
    static void BeginBatch();
    static void EndBatch();
    static void Flush();
};