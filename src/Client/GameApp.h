#pragma once

#include <memory>
#include "Platform/Window/Window.h"
#include "Platform/OpenGL/Shader.h"
#include "Engine/Renderer/OrthographicCamera.h"

class GameApp {
  public:
    GameApp();
    ~GameApp();

    void Run();

  private:
    void Update();
    void Render();

  private:
    std::unique_ptr<Window> m_Window;
    bool m_Running = true;

    std::unique_ptr<Shader> m_Shader;
    unsigned int m_VAO, m_VBO;

    std::unique_ptr<OrthographicCamera> m_Camera;
};
