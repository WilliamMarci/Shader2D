#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/Application.h"
#include "Engine/Core/Log.h"

#ifdef ENG_PLATFORM_WINDOWS

extern Engine::Application* Engine::CreateApplication();

int main(int argc, char** argv) {
    Engine::Log::Init();
    ENG_CORE_INFO("Initialized Log!");

    auto app = Engine::CreateApplication();

    app->Run();

    delete app;

    return 0;
}

#elif defined(ENG_PLATFORM_LINUX) || defined(ENG_PLATFORM_MACOS)

extern Engine::Application* Engine::CreateApplication();

int main(int argc, char** argv) {
    Engine::Log::Init();
    ENG_CORE_INFO("Initialized Log!");

    auto app = Engine::CreateApplication();
    app->Run();
    delete app;
    return 0;
}

#else
    #error "Unsupport Platform!"
#endif
