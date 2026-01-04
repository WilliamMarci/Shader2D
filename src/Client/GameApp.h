#pragma once

#include <memory>
#include <vector>

// 引入 Engine 核心组件
#include "Platform/Window/Window.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Renderer/OrthographicCamera.h"

class GameApp {
public:
    GameApp();
    ~GameApp();

    void Run(); 

private:
    void Init(); 
    void OnUpdate(float ts); // (ts = timestep/deltaTime) logic update
    void OnRender(); // rendering update

private:
    bool m_Running = true;
    std::unique_ptr<Window> m_Window;

    std::shared_ptr<Shader> m_Shader;
    std::shared_ptr<Texture2D> m_Texture; 


    std::shared_ptr<OrthographicCamera> m_Camera;

    glm::vec3 m_CameraPosition = {0.0f, 0.0f, 0.0f};
    float m_CameraRotation = 0.0f;
    float m_CameraZoom = 1.0f;
    float m_CameraSpeed = 2.0f;

    float m_LastFrameTime = 0.0f;
};
