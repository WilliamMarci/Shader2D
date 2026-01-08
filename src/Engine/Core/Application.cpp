#include "Application.h"

#include "Engine/Core/Log.h"
#include "Engine/Core/Timestep.h"
#include "Engine/Renderer/Renderer2D.h"
// 临时使用 GLFW 获取时间，后续可以封装到 Platform/Time
#include <GLFW/glfw3.h> 

namespace Engine {

    Application* Application::s_Instance = nullptr;

    Application::Application() {
        ENG_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        // 创建窗口
        m_Window = Window::Create();
        
        // 绑定事件回调
        m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
        Renderer2D::Init();
    }

    Application::~Application() {
    }

    void Application::PushLayer(Layer* layer) {
        m_LayerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer* layer) {
        m_LayerStack.PushOverlay(layer);
    }

    void Application::OnEvent(Event& e) {
        EventDispatcher dispatcher(e);
        
        dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
        dispatcher.Dispatch<WindowResizeEvent>(std::bind(&Application::OnWindowResize, this, std::placeholders::_1));

        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
            if (e.Handled) 
                break;
            (*it)->OnEvent(e);
        }
    }

    void Application::Run() {
        while (m_Running) {
            float time = (float)glfwGetTime(); 
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            if (!m_Minimized) {
                for (Layer* layer : m_LayerStack)
                    layer->OnUpdate(timestep);
            }

            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e) {
        m_Running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e) {
        if (e.GetWidth() == 0 || e.GetHeight() == 0) {
            m_Minimized = true;
            return false;
        }

        m_Minimized = false;
        
        
        return false;
    }

}
