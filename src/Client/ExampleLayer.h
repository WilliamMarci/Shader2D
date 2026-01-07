#pragma once

#include "Engine/Core/Layer.h"
#include "Engine/Core/Timestep.h"

#include "Engine/Renderer/OrthographicCamera.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/VertexArray.h"

// 包含 glm
#include <glm/glm.hpp>

class ExampleLayer : public Engine::Layer {
public:
    ExampleLayer();
    virtual ~ExampleLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    virtual void OnUpdate(Engine::Timestep ts) override;
    virtual void OnEvent(Engine::Event& event) override;

private:
    // 渲染资源
    std::shared_ptr<Engine::Shader> m_Shader;
    std::shared_ptr<Engine::VertexArray> m_VertexArray; // 如果需要的话
    std::shared_ptr<Engine::Texture2D> m_Texture;

    // 相机系统
    std::shared_ptr<Engine::OrthographicCamera> m_Camera;
    glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
    float m_CameraRotation = 0.0f;
    
    // 相机控制参数
    float m_CameraSpeed = 5.0f;
    float m_CameraZoom = 1.0f;
};
