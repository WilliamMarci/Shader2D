#include "GameApp.h"
#include "Platform/Input/Input.h"
#include "Platform/OpenGL/BatchRenderer.h" // 引入新工具
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream> // 用于打印统计信息

GameApp::GameApp() {
    m_Window = std::make_unique<Window>("My Indie Engine", 1600, 1200);
    Input::Init(m_Window.get());

    // 1. 初始化渲染器
    BatchRenderer::Init();
    float aspectRatio = 800.0f / 600.0f;
    m_Camera = std::make_unique<OrthographicCamera>(aspectRatio);
    // 2. 加载 Shader
    m_Shader = std::make_unique<Shader>(
        "assets/engine/shaders/core_default.vert",
        "assets/engine/shaders/core_default.frag"
    );
}

GameApp::~GameApp() {
    // 清理
    BatchRenderer::Shutdown();
}

void GameApp::Run() {
    while (m_Running && !m_Window->ShouldClose()) {
        Update();
        Render();
        m_Window->Update();
    }
}

void GameApp::Update() {
    if (Input::IsKeyPressed(GLFW_KEY_ESCAPE)) {
        m_Running = false;
    }
    
    glm::vec3 pos = m_Camera->GetPosition();
    float rot = m_Camera->GetRotation();
    float zoom = m_Camera->GetZoomLevel();

    
    float speed = 2.0f * 0.016f * m_Camera->GetZoomLevel(); 
    if (Input::IsKeyPressed(GLFW_KEY_A)) pos.x -= speed;
    if (Input::IsKeyPressed(GLFW_KEY_D)) pos.x += speed;
    if (Input::IsKeyPressed(GLFW_KEY_W)) pos.y += speed;
    if (Input::IsKeyPressed(GLFW_KEY_S)) pos.y -= speed;
    if (Input::IsKeyPressed(GLFW_KEY_Q)) rot += 1.0f;
    if (Input::IsKeyPressed(GLFW_KEY_E)) rot -= 1.0f;
    if (Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) zoom += 1.0f * 0.016f;
    if (Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL)) zoom -= 1.0f * 0.016f;
    zoom = glm::max(zoom, 0.25f);

    m_Camera->SetPosition(pos);
    m_Camera->SetRotation(rot);
    m_Camera->SetZoomLevel(zoom);

    float aspectRatio = (float)m_Window->GetWidth() / (float)m_Window->GetHeight();
    m_Camera->SetProjection(aspectRatio, zoom);
    BatchRenderer::ResetStats();
}

void GameApp::Render() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_Shader->Bind();

    // --- 批处理绘制流程 ---
    BatchRenderer::BeginScene(*m_Camera, *m_Shader);

    // 画一个红色的正方形
    BatchRenderer::DrawQuad({-0.5f, -0.5f}, {0.5f, 0.5f}, {1.0f, 0.0f, 0.0f, 1.0f});
    
    // 画一个绿色的小矩形
    BatchRenderer::DrawQuad({0.2f, 0.2f}, {0.2f, 0.3f}, {0.0f, 1.0f, 0.0f, 1.0f});

    // 压力测试：循环画 10000 个
    for(float x = -0.9f; x < 9.0f; x += 0.05f) {
        for(float y = 0.5f; y < 9.0f; y += 0.05f) {
             BatchRenderer::DrawQuad({x, y}, {0.04f, 0.04f}, {0.0f, 0.0f, 1.0f, 1.0f});
        }
    }

    BatchRenderer::EndScene();
    
    // 可选：打印统计信息 (每 60 帧打印一次，防止刷屏)
    // std::cout << "Draw Calls: " << BatchRenderer::GetStats().DrawCalls << std::endl;
}
