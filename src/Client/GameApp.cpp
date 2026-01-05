#include "GameApp.h"

// 引入 Engine 接口
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Renderer/RenderCommand.h"
#include "Platform/Input/Input.h"

// 仍然需要 GLFW 的按键宏 (除非你在 Input.h 里自定义了 KeyCode)
#include <GLFW/glfw3.h> 
#include <iostream>

GameApp::GameApp() {
    // 1. 创建窗口
    m_Window = std::make_unique<Window>("My Indie Engine", 1600, 1200);
    Input::Init(m_Window.get());

    // 2. 初始化渲染系统
    Renderer2D::Init();

    // 3. 初始化摄像机 (宽高比)
    float aspectRatio = (float)m_Window->GetWidth() / (float)m_Window->GetHeight();
    m_Camera = std::make_shared<OrthographicCamera>(aspectRatio);
    
    // 4. 加载资源
    Init();
}

GameApp::~GameApp() {
    Renderer2D::Shutdown();
}

void GameApp::Init() {
    // 使用工厂方法创建 Shader (跨平台)
    m_Shader = Shader::Create(
        "assets/engine/shaders/core_default.vert",
        "assets/engine/shaders/core_default.frag"
    );

    // 如果你有纹理，也可以在这里加载
    m_Texture = Texture2D::Create("assets/game/icons/shader.png");

    // 绑定 Shader 并设置纹理槽位 (假设 Shader 里有个 u_Textures 数组)
    m_Shader->Bind();
    // 这一步通常由 Renderer2D 自动处理，但如果 Shader 需要初始设置可以写在这
}

void GameApp::Run() {
    while (m_Running && !m_Window->ShouldClose()) {
        // 计算时间差 (Delta Time)
        float time = (float)glfwGetTime(); // 或者 Window::GetTime()
        float timestep = time - m_LastFrameTime;
        m_LastFrameTime = time;

        OnUpdate(timestep);
        OnRender();
        
        m_Window->Update();
    }
}

void GameApp::OnUpdate(float ts) {
    if (Input::IsKeyPressed(GLFW_KEY_ESCAPE)) {
        m_Running = false;
    }

    // --- 摄像机控制逻辑 ---
    // 注意：速度乘以 ts (timestep) 以保证帧率无关性
    if (Input::IsKeyPressed(GLFW_KEY_A)) m_CameraPosition.x -= m_CameraSpeed * ts * m_CameraZoom;
    if (Input::IsKeyPressed(GLFW_KEY_D)) m_CameraPosition.x += m_CameraSpeed * ts * m_CameraZoom;
    if (Input::IsKeyPressed(GLFW_KEY_W)) m_CameraPosition.y += m_CameraSpeed * ts * m_CameraZoom;
    if (Input::IsKeyPressed(GLFW_KEY_S)) m_CameraPosition.y -= m_CameraSpeed * ts * m_CameraZoom;

    if (Input::IsKeyPressed(GLFW_KEY_Q)) m_CameraRotation += 90.0f * ts;
    if (Input::IsKeyPressed(GLFW_KEY_E)) m_CameraRotation -= 90.0f * ts;

    // 缩放控制
    if (Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) m_CameraZoom += 2.0f * ts;
    if (Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL)) m_CameraZoom -= 2.0f * ts;
    m_CameraZoom = std::max(m_CameraZoom, 0.25f);

    // 应用变换到摄像机
    m_Camera->SetPosition(m_CameraPosition);
    m_Camera->SetRotation(m_CameraRotation);
    
    // 处理窗口大小变化导致的纵横比改变
    float aspectRatio = (float)m_Window->GetWidth() / (float)m_Window->GetHeight();
    m_Camera->SetProjection(aspectRatio, m_CameraZoom);

    // 重置统计数据
    Renderer2D::ResetStats();
}

void GameApp::OnRender() {
    // 1. 清屏 (使用 RenderCommand 抽象层)
    RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    RenderCommand::Clear();

    // 2. 开始场景 (传入摄像机和 Shader)
    // Renderer2D 会自动提取 ViewProjection 矩阵传给 Shader
    Renderer2D::BeginScene(*m_Camera, *m_Shader);

    // --- 绘制命令 ---

    // 画一个红色的正方形
    Renderer2D::DrawQuad({-0.5f, -0.5f}, {0.5f, 0.5f}, {1.0f, 0.0f, 0.0f, 1.0f});
    
    // 画一个绿色的小矩形
    Renderer2D::DrawQuad({0.2f, 0.2f}, {0.2f, 0.3f}, {0.0f, 1.0f, 0.0f, 1.0f});

    // 压力测试：循环画 10000 个蓝色小方块
    // (由于使用了 Batch Rendering，这依然只会产生 1-2 个 Draw Call，非常快)
    for(float x = -5.0f; x < 5.0f; x += 0.1f) {
        for(float y = -5.0f; y < 5.0f; y += 0.1f) {
             Renderer2D::DrawQuad({x, y}, {0.08f, 0.08f}, {0.0f, 0.0f, 1.0f, 0.5f});
        }
    }

    // Draw 带纹理的四边形 (如果你加载了纹理)
    Renderer2D::DrawQuad({1.0f, 0.0f}, {1.0f, 1.0f}, m_Texture, 1.0f, {1.0f, 1.0f, 1.0f, 1.0f});
    // m_Texture->Bind(0); // 解绑纹理

    // 3. 结束场景 (提交 GPU)
    Renderer2D::EndScene();

    // 可选：打印统计信息
    // std::cout << "Draw Calls: " << Renderer2D::GetStats().DrawCalls 
    //           << " Quads: " << Renderer2D::GetStats().QuadCount << std::endl;
}
