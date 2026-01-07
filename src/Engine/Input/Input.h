#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Input/KeyCodes.h" // 引入你的 KeyCode 定义
#include <utility>

namespace Engine {

    class Input {
    public:
        static bool IsKeyPressed(KeyCode keycode);
        static bool IsMouseButtonPressed(MouseCode button);
        static std::pair<float, float> GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();
    };

}
