#include "BatchRenderer.h"
#include <glad/glad.h>
#include <array>
#include <cstring>

const size_t MAX_QUADS = 10000;
const size_t MAX_VERTICES = MAX_QUADS * 4;
const size_t MAX_INDICES = MAX_QUADS * 6;

struct RendererData {
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;

    std::unique_ptr<Vertex[]> QuadBufferBase;
    Vertex* QuadBufferPrt = nullptr;

    unsigned int IndexCount = 0;

    glm::vec2 CameraMin;
    glm::vec2 CameraMax;

    RendererStats Stats;
};

static RendererData s_Data;

void BatchRenderer::Init() {
    s_Data.QuadBufferBase = std::make_unique<Vertex[]>(MAX_VERTICES);

    glGenVertexArrays(1, &s_Data.VAO);
    glBindVertexArray(s_Data.VAO);

    glGenBuffers(1, &s_Data.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, s_Data.VBO);
    glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<const void*>(offsetof(Vertex, Position)));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<const void*>(offsetof(Vertex, Color)));
    // EBO cache
    auto indices = std::make_unique<uint32_t[]>(MAX_INDICES);
    uint32_t offset = 0;
    for (size_t q = 0; q < MAX_QUADS; q++) {
        uint32_t offset = q * 4;
        uint32_t idx = q * 6;
        // A quad is build by 2 triangle:
        // tri:0--1--2 and tri:1--2--3 -> quad:0--1--2--3
        indices[idx + 0] = offset + 0;
        indices[idx + 1] = offset + 1;
        indices[idx + 2] = offset + 2;
        indices[idx + 3] = offset + 2;
        indices[idx + 4] = offset + 3;
        indices[idx + 5] = offset + 0;
    }

    glGenBuffers(1, &s_Data.EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_INDICES * sizeof(uint32_t), indices.get(), GL_STATIC_DRAW);
}

void BatchRenderer::Shutdown() {
    glDeleteVertexArrays(1, &s_Data.VAO);
    glDeleteBuffers(1, &s_Data.VBO);
    glDeleteBuffers(1, &s_Data.EBO);
}

void BatchRenderer::BeginScene(const Camera& camera, Shader& shader) {
    shader.Bind();
    shader.SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
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

void BatchRenderer::EndScene() { EndBatch(); }

bool BatchRenderer::IsOnScreen(const glm::vec2& pos, const glm::vec2& size) {
    //AABB check
    float objMinX = std::min(pos.x, pos.x + size.x);
    float objMaxX = std::max(pos.x, pos.x + size.x);
    float objMinY = std::min(pos.y, pos.y + size.y);
    float objMaxY = std::max(pos.y, pos.y + size.y);

    if (objMaxX < s_Data.CameraMin.x) return false;
    if (objMinX > s_Data.CameraMax.x) return false;
    if (objMaxY < s_Data.CameraMin.y) return false;
    if (objMinY > s_Data.CameraMax.y) return false;
    
    return true;
}

void BatchRenderer::BeginBatch() {
    s_Data.QuadBufferPrt = s_Data.QuadBufferBase.get();
    s_Data.IndexCount = 0;
}
void BatchRenderer::EndBatch() {
    GLsizeiptr size = (uint8_t*)s_Data.QuadBufferPrt - (uint8_t*)s_Data.QuadBufferBase.get();
    if (size > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, s_Data.VBO);
        void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        memcpy(ptr, s_Data.QuadBufferBase.get(), size);
        glUnmapBuffer(GL_ARRAY_BUFFER);

        Flush();
    }
}

void BatchRenderer::Flush() {
    glBindVertexArray(s_Data.VAO);
    glDrawElements(GL_TRIANGLES, s_Data.IndexCount, GL_UNSIGNED_INT, nullptr);
    s_Data.Stats.DrawCalls++;
}

void BatchRenderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
    if (!IsOnScreen(position, size)) return;

    if (s_Data.IndexCount >= MAX_INDICES) {
        EndBatch();
        Flush();
        BeginBatch();
    }
    // Draw a quad
    //  (0)-----(1)
    //   |       |
    //  (3)-----(2)
    s_Data.QuadBufferPrt->Position = {position.x, position.y, 0.0f};
    s_Data.QuadBufferPrt->Color = color;
    s_Data.QuadBufferPrt++;

    s_Data.QuadBufferPrt->Position = {position.x + size.x, position.y, 0.0f};
    s_Data.QuadBufferPrt->Color = color;
    s_Data.QuadBufferPrt++;

    s_Data.QuadBufferPrt->Position = {position.x + size.x, position.y + size.y, 0.0f};
    s_Data.QuadBufferPrt->Color = color;
    s_Data.QuadBufferPrt++;

    s_Data.QuadBufferPrt->Position = {position.x, position.y + size.y, 0.0f};
    s_Data.QuadBufferPrt->Color = color;
    s_Data.QuadBufferPrt++;

    s_Data.IndexCount += 6;
    s_Data.Stats.QuadCount++;
};

RendererStats BatchRenderer::GetStats() { return s_Data.Stats; }

void BatchRenderer::ResetStats() { memset(&s_Data.Stats, 0, sizeof(RendererStats)); }