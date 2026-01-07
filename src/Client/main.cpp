#include "Engine/Core/Application.h"
#include "Engine/Core/EntryPoint.h"

#include "ExampleLayer.h"

class GameApp : public Engine::Application {
public:
    GameApp() {
        // 只需要把 Layer 推入栈中
        PushLayer(new ExampleLayer());
    }

    ~GameApp() {
    }
};

// 定义入口点工厂函数
Engine::Application* Engine::CreateApplication() {
    return new GameApp();
}
