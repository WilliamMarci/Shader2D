#include "Renderer2D.h"

#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>
#include <array>
#include <cstring>

const size_t MAX_QUADS = 10000;
const size_t MAX_VERTICES = MAX_QUADS * 4;
const size_t MAX_INDICES = MAX_QUADS * 6;

struct RendererData {
    std::shared_ptr<VertexArray> QuadVertexArray;
    std::shared_ptr<VertexBuffer> QuadVertexBuffer;
    std::shared_ptr<Shader> TextureShader;

    std::unique_ptr<Vertex[]> QuadBufferBase;
    Vertex* QuadBufferPtr = nullptr;

    uint32_t IndexCount = 0;

    glm::vec2 CameraMin;
    glm::vec2 CameraMax;

    RendererStats Stats;
};

static RendererData s_Data;

void Renderer2D::Init() {
    s_Data.QuadVertexArray = VertexArray::Create();

    s_Data.QuadVertexBuffer = VertexBuffer::Create(MAX_VERTICES * sizeof(Vertex));
    
    s_Data.QuadVertexBuffer->SetLayout({
        { ShaderDataType::Float3, "a_Position" },
        { ShaderDataType::Float4, "a_Color" }
    });
    
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
}

void Renderer2D::Shutdown() {
    // Smart pointers will release resources automatically
    s_Data.QuadVertexArray.reset();
    s_Data.QuadVertexBuffer.reset();
    s_Data.TextureShader.reset();
    s_Data.QuadBufferBase.reset();
}

void Renderer2D::BeginScene(const Camera& camera, Shader& shader) {
    shader.Bind();
    shader.SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

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
    EndBatch();
}

void Renderer2D::BeginBatch() {
    s_Data.QuadBufferPtr = s_Data.QuadBufferBase.get();
    s_Data.IndexCount = 0;
}

void Renderer2D::EndBatch() {
    uint32_t dataSize = (uint8_t*)s_Data.QuadBufferPtr - (uint8_t*)s_Data.QuadBufferBase.get();
    
    if (dataSize > 0) {
        // Upload data to GPU
        s_Data.QuadVertexBuffer->SetData(s_Data.QuadBufferBase.get(), dataSize);

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

RendererStats& Renderer2D::GetStats() {
    return s_Data.Stats;
}

void Renderer2D::ResetStats() {
    memset(&s_Data.Stats, 0, sizeof(RendererStats));
}