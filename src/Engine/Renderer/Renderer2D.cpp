#include "Renderer2D.h"

#include "Engine/Renderer/RenderCommand.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/VertexArray.h"

#include <glm/gtc/matrix_transform.hpp>
#include <array>
#include <cstring>
#include <iostream>

const size_t MAX_QUADS = 10000;
const size_t MAX_VERTICES = MAX_QUADS * 4;
const size_t MAX_INDICES = MAX_QUADS * 6;
const size_t MAX_TEXTURE_SLOTS = 32; // for openGL < 4.x

struct RendererData {
    std::shared_ptr<VertexArray> QuadVertexArray;
    std::shared_ptr<VertexBuffer> QuadVertexBuffer;
    std::shared_ptr<Shader> TextureShader;
    std::shared_ptr<Texture2D> WhiteTexture;

    std::unique_ptr<Vertex[]> QuadBufferBase;
    Vertex* QuadBufferPtr = nullptr;

    uint32_t IndexCount = 0;

    std::array<std::shared_ptr<Texture2D>, MAX_TEXTURE_SLOTS> TextureSlots;
    uint32_t TextureSlotIndex = 1;

    glm::vec2 CameraMin;
    glm::vec2 CameraMax;

    RendererStats Stats;
};

static RendererData s_Data;

void Renderer2D::Init() {
    s_Data.QuadVertexArray = VertexArray::Create();

    s_Data.QuadVertexBuffer = VertexBuffer::Create(MAX_VERTICES * sizeof(Vertex));

    s_Data.QuadVertexBuffer->SetLayout({{ShaderDataType::Float3, "a_Position"},
                                        {ShaderDataType::Float4, "a_Color"},
                                        {ShaderDataType::Float2, "a_TexCoord"},
                                        {ShaderDataType::Float, "a_TexIndex"}});

    s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
    s_Data.QuadBufferBase = std::make_unique<Vertex[]>(MAX_VERTICES);

    // Create Index Buffer
    auto indices = std::make_unique<uint32_t[]>(MAX_INDICES);
    uint32_t offset = 0;
    for (size_t q = 0; q < MAX_QUADS; q++) {
        uint32_t offset = q * 4;
        uint32_t idx = q * 6;
        // 0->1->2, 2->3->0
        indices[idx + 0] = offset + 0;
        indices[idx + 1] = offset + 1;
        indices[idx + 2] = offset + 2;
        indices[idx + 3] = offset + 2;
        indices[idx + 4] = offset + 3;
        indices[idx + 5] = offset + 0;
    }

    std::shared_ptr<IndexBuffer> quadIB = IndexBuffer::Create(indices.get(), MAX_INDICES);
    s_Data.QuadVertexArray->SetIndexBuffer(quadIB);

    s_Data.WhiteTexture = Texture2D::Create(1, 1);
    uint32_t whiteTextureData = 0xffffffff;
    s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
    s_Data.TextureSlots[0] = s_Data.WhiteTexture;
}

void Renderer2D::Shutdown() {
    // Smart pointers will release resources automatically
    s_Data.QuadVertexArray.reset();
    s_Data.QuadVertexBuffer.reset();
    s_Data.TextureShader.reset();
    s_Data.WhiteTexture.reset();
    s_Data.QuadBufferBase.reset();

    for (auto& texture : s_Data.TextureSlots) {
        texture.reset();
    }
}

void Renderer2D::BeginScene(const Camera& camera, Shader& shader) {
    shader.Bind();
    shader.SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

    int samplers[MAX_TEXTURE_SLOTS];
    for (uint32_t i = 0; i < MAX_TEXTURE_SLOTS; i++) {
        samplers[i] = i;
    }
    shader.SetIntArray("u_Textures", samplers, MAX_TEXTURE_SLOTS);
    // Calculate camera bounds for culling
    glm::mat4 invViewProj = glm::inverse(camera.GetViewProjectionMatrix());
    glm::vec4 corners[4] = {
        {-1.0f, -1.0f, 0.0f, 1.0f},
        {1.0f, -1.0f, 0.0f, 1.0f},
        {1.0f, 1.0f, 0.0f, 1.0f},
        {-1.0f, 1.0f, 0.0f, 1.0f},
    };

    s_Data.CameraMin = {FLT_MAX, FLT_MAX};
    s_Data.CameraMax = {-FLT_MAX, -FLT_MAX};

    for (size_t i = 0; i < 4; i++) {
        glm::vec4 worldPos = invViewProj * corners[i];
        worldPos /= worldPos.w;

        s_Data.CameraMin.x = glm::min(s_Data.CameraMin.x, worldPos.x);
        s_Data.CameraMin.y = glm::min(s_Data.CameraMin.y, worldPos.y);
        s_Data.CameraMax.x = glm::max(s_Data.CameraMax.x, worldPos.x);
        s_Data.CameraMax.y = glm::max(s_Data.CameraMax.y, worldPos.y);
    }

    BeginBatch();
}

void Renderer2D::EndScene() { 
    // std::cout << "[Renderer2D] EndScene called. Flushing batch..." << std::endl;
    EndBatch(); }

void Renderer2D::BeginBatch() {
    s_Data.QuadBufferPtr = s_Data.QuadBufferBase.get();
    s_Data.IndexCount = 0;
    s_Data.TextureSlotIndex = 1;
}

void Renderer2D::EndBatch() {
    uint32_t dataSize = (uint8_t*)s_Data.QuadBufferPtr - (uint8_t*)s_Data.QuadBufferBase.get();

    if (dataSize > 0) {
        // Upload data to GPU
        s_Data.QuadVertexBuffer->SetData(s_Data.QuadBufferBase.get(), dataSize);

        for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++) {
            s_Data.TextureSlots[i]->Bind(i);
        }
        // Draw
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.IndexCount);
        s_Data.Stats.DrawCalls++;
    }
}

void Renderer2D::Flush() {
    // If we had textures, we would bind them here
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
    DrawQuad({position.x, position.y, 0.0f}, size, color);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
    const float texIndex = 0.0f; // White Texture
    const float tilingFactor = 1.0f;
    if (s_Data.IndexCount >= MAX_INDICES) {
        EndBatch();
        BeginBatch();
    }

    if (!IsOnScreen({position.x, position.y}, size)) {
        return; // Culling
    }

    // Quad
    // (3)-----(2)
    //  |       |
    // (0)-----(1)
    s_Data.QuadBufferPtr->Position = {position.x - size.x * 0.5f, position.y - size.y * 0.5f, 0.0f};
    s_Data.QuadBufferPtr->Color = color;
    s_Data.QuadBufferPtr++;

    s_Data.QuadBufferPtr->Position = {position.x + size.x * 0.5f, position.y - size.y * 0.5f, 0.0f};
    s_Data.QuadBufferPtr->Color = color;
    s_Data.QuadBufferPtr++;

    s_Data.QuadBufferPtr->Position = {position.x + size.x * 0.5f, position.y + size.y * 0.5f, 0.0f};
    s_Data.QuadBufferPtr->Color = color;
    s_Data.QuadBufferPtr++;

    s_Data.QuadBufferPtr->Position = {position.x - size.x * 0.5f, position.y + size.y * 0.5f, 0.0f};
    s_Data.QuadBufferPtr->Color = color;
    s_Data.QuadBufferPtr++;

    s_Data.IndexCount += 6;
    s_Data.Stats.QuadCount++;
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture,
                          float tilingFactor, const glm::vec4& tintColor) {
    DrawQuad({position.x, position.y, 0.0f}, size, texture, tilingFactor, tintColor);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture,
                          float tilingFactor, const glm::vec4& tintColor) {
    if (!texture) {
        DrawQuad(position, size, {1.0f, 0.0f, 0.5f, 1.0f}); // Fallback color
        return;
    }
    if (s_Data.IndexCount >= MAX_INDICES) {
        EndBatch();
        BeginBatch();
    }

    float textureIndex = 0.0f;
    for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
        if (*s_Data.TextureSlots[i] == *texture) {
            textureIndex = (float)i;
            break;
        }
    }

    if (textureIndex == 0.0f) {
        if (s_Data.TextureSlotIndex >= MAX_TEXTURE_SLOTS) {
            EndBatch();
            BeginBatch();
        }
        textureIndex = (float)s_Data.TextureSlotIndex;
        s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
        s_Data.TextureSlotIndex++;
    }

    if (!IsOnScreen({position.x, position.y}, size)) return;

    s_Data.QuadBufferPtr->Position = {position.x - size.x * 0.5f, position.y - size.y * 0.5f, 0.0f};
    s_Data.QuadBufferPtr->Color = tintColor;
    s_Data.QuadBufferPtr->TexCoord = {0.0f, 0.0f};
    s_Data.QuadBufferPtr->TexIndex = textureIndex;
    s_Data.QuadBufferPtr++;

    s_Data.QuadBufferPtr->Position = {position.x + size.x * 0.5f, position.y - size.y * 0.5f, 0.0f};
    s_Data.QuadBufferPtr->Color = tintColor;
    s_Data.QuadBufferPtr->TexCoord = {1.0f * tilingFactor, 0.0f};
    s_Data.QuadBufferPtr->TexIndex = textureIndex;
    s_Data.QuadBufferPtr++;

    s_Data.QuadBufferPtr->Position = {position.x + size.x * 0.5f, position.y + size.y * 0.5f, 0.0f};
    s_Data.QuadBufferPtr->Color = tintColor;
    s_Data.QuadBufferPtr->TexCoord = {1.0f * tilingFactor, 1.0f * tilingFactor};
    s_Data.QuadBufferPtr->TexIndex = textureIndex;
    s_Data.QuadBufferPtr++;

    s_Data.QuadBufferPtr->Position = {position.x - size.x * 0.5f, position.y + size.y * 0.5f, 0.0f};
    s_Data.QuadBufferPtr->Color = tintColor;
    s_Data.QuadBufferPtr->TexCoord = {0.0f, 1.0f * tilingFactor};
    s_Data.QuadBufferPtr->TexIndex = textureIndex;
    s_Data.QuadBufferPtr++;

    s_Data.IndexCount += 6;
    s_Data.Stats.QuadCount++;
}

bool Renderer2D::IsOnScreen(const glm::vec2& pos, const glm::vec2& size) {
    float objMinX = pos.x - size.x * 0.5f;
    float objMaxX = pos.x + size.x * 0.5f;
    float objMinY = pos.y - size.y * 0.5f;
    float objMaxY = pos.y + size.y * 0.5f;

    if (objMaxX < s_Data.CameraMin.x) return false;
    if (objMinX > s_Data.CameraMax.x) return false;
    if (objMaxY < s_Data.CameraMin.y) return false;
    if (objMinY > s_Data.CameraMax.y) return false;

    return true;
}

RendererStats& Renderer2D::GetStats() { return s_Data.Stats; }

void Renderer2D::ResetStats() { memset(&s_Data.Stats, 0, sizeof(RendererStats)); }