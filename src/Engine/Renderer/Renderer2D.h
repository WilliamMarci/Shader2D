#pragma once

#include "Engine/Renderer/Camera.h"
#include "Engine/Renderer/Texture.h" 
#include "Engine/Renderer/Shader.h"
#include "Engine/Core/Math.h"
#include <memory>
#include <vector>

namespace Engine {

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
    static void DrawQuad(const Vec2& position, const Vec2& size, const Vec4& color);
    static void DrawQuad(const Vec3& position, const Vec2& size, const Vec4& color);
    
    // Textured Quad
    static void DrawQuad(const Vec2& position, const Vec2& size, const std::shared_ptr<Texture2D>& texture, float tilingFactor = 1.0f, const Vec4& tintColor = Vec4(1.0f));
    static void DrawQuad(const Vec3& position, const Vec2& size, const std::shared_ptr<Texture2D>& texture, float tilingFactor = 1.0f, const Vec4& tintColor = Vec4(1.0f));
    
    static void DrawRotatedQuad(const Vec2& position, const Vec2& size, float rotation, const Vec4& color);
    static void DrawRotatedQuad(const Vec3& position, const Vec2& size, float rotation, const Vec4& color);
    static void DrawRotatedQuad(const Vec2& position, const Vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture, float tilingFactor = 1.0f, const Vec4& tintColor = Vec4(1.0f));
    static void DrawRotatedQuad(const Vec3& position, const Vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture, float tilingFactor = 1.0f, const Vec4& tintColor = Vec4(1.0f));

    static RendererStats& GetStats();
    static void ResetStats();
    
    private:
    static void Flush();
    static void BeginBatch();
    static void EndBatch();
    static bool IsOnScreen(const Vec2& pos, const Vec2& size);
};

}