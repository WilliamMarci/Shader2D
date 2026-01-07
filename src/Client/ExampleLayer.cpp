#include "ExampleLayer.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Renderer/RenderCommand.h"
#include "Engine/Input/Input.h"
#include "Engine/Input/KeyCodes.h" // 引入 KeyCode 定义

// 方便使用 Engine 命名空间
using namespace Engine;

ExampleLayer::ExampleLayer()
    : Layer("Example"), 
      m_Camera(std::make_shared<OrthographicCamera>(1280.0f / 720.0f)) // 假设初始比例
{
}

void ExampleLayer::OnAttach() {
    // 1. 加载 Shader
    // 注意：路径可能需要根据你的运行目录调整，通常是相对于项目根目录或 bin 目录
    m_Shader = Shader::Create(
        "assets/engine/shaders/core_default.vert",
        "assets/engine/shaders/core_default.frag"
    );

    // 2. 加载纹理
    m_Texture = Texture2D::Create("assets/game/icons/shader.png");

    // 3. 可以在这里做一些初始设置
    // m_Shader->Bind(); // 如果需要预绑定
}

void ExampleLayer::OnDetach() {
    // 资源通常由 shared_ptr 自动释放，或者在这里手动 Shutdown
}

void ExampleLayer::OnUpdate(Timestep ts) {
    // --- 1. 相机控制逻辑 (从原 GameApp::OnUpdate 迁移) ---
    
    // 移动
    if (Input::IsKeyPressed(KeyCode::A)) 
        m_CameraPosition.x -= m_CameraSpeed * ts * m_CameraZoom;
    if (Input::IsKeyPressed(KeyCode::D)) 
        m_CameraPosition.x += m_CameraSpeed * ts * m_CameraZoom;
    if (Input::IsKeyPressed(KeyCode::W)) 
        m_CameraPosition.y += m_CameraSpeed * ts * m_CameraZoom;
    if (Input::IsKeyPressed(KeyCode::S)) 
        m_CameraPosition.y -= m_CameraSpeed * ts * m_CameraZoom;

    // 旋转
    if (Input::IsKeyPressed(KeyCode::Q)) 
        m_CameraRotation += 90.0f * ts;
    if (Input::IsKeyPressed(KeyCode::E)) 
        m_CameraRotation -= 90.0f * ts;

    // 缩放
    if (Input::IsKeyPressed(KeyCode::LeftShift)) // 假设你没有定义 LeftShift，用 Shift 也可以
        m_CameraZoom += 2.0f * ts;
    if (Input::IsKeyPressed(KeyCode::LeftControl))
        m_CameraZoom -= 2.0f * ts;
    
    m_CameraZoom = std::max(m_CameraZoom, 0.25f);

    // 应用变换
    m_Camera->SetPosition(m_CameraPosition);
    m_Camera->SetRotation(m_CameraRotation);
    
    // 设置投影 (处理缩放)
    // 注意：这里暂时硬编码了宽高比，理想情况下应该从 Application::Get().GetWindow() 获取
    float aspectRatio = 1280.0f / 720.0f; 
    m_Camera->SetProjection(aspectRatio, m_CameraZoom);


    // --- 2. 渲染逻辑 (从原 GameApp::OnRender 迁移) ---

    // 重置统计
    Renderer2D::ResetStats();

    // 清屏
    RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    RenderCommand::Clear();

    // 开始场景 (传入 Shader)
    Renderer2D::BeginScene(*m_Camera, *m_Shader);

    // --- 绘制命令 ---

    // 红色正方形
    Renderer2D::DrawQuad({-0.5f, -0.5f}, {0.5f, 0.5f}, {1.0f, 0.0f, 0.0f, 1.0f});
    
    // 绿色小矩形
    Renderer2D::DrawQuad({0.2f, 0.2f}, {0.2f, 0.3f}, {0.0f, 1.0f, 0.0f, 1.0f});

    // 压力测试：10000 个蓝色小方块
    for(float x = -5.0f; x < 5.0f; x += 0.1f) {
        for(float y = -5.0f; y < 5.0f; y += 0.1f) {
             Renderer2D::DrawQuad({x, y}, {0.08f, 0.08f}, {0.0f, 0.0f, 1.0f, 0.5f});
        }
    }

    // 带纹理的四边形
    if (m_Texture) {
        Renderer2D::DrawQuad({1.0f, 0.0f}, {1.0f, 1.0f}, m_Texture, 1.0f, {1.0f, 1.0f, 1.0f, 1.0f});
    }

    // 结束场景
    Renderer2D::EndScene();
}

void ExampleLayer::OnEvent(Event& event) {
    // 这里可以处理窗口大小变化事件来更新 aspectRatio
}
