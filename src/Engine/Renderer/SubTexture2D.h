#pragma once

#include "Engine/Core/Math.h"
#include "Engine/Renderer/Texture.h"

#include <memory>

namespace Engine {
class SubTexture2D {
  public:
    SubTexture2D(const std::shared_ptr<Texture2D>& texture, const Vec2& min, const Vec2& max);
    const std::shared_ptr<Texture2D>& GetTexture() const { return m_Texture; }
    const Vec2* GetTexCoords() const { return m_TexCoords; }

    // coords: (u, v) 
    static std::shared_ptr<SubTexture2D> CreateFromCoords(const std::shared_ptr<Texture2D>& texture, const Vec2& coords,
                                                          const Vec2& cellSize,
                                                          const Vec2& spriteSize = Vec2(1.0f, 1.0f));
  private:
    std::shared_ptr<Texture2D> m_Texture;
    Vec2 m_TexCoords[4];
};
} // namespace Engine
