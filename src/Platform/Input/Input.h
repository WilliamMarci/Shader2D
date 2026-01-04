#pragma once

#include "Platform/Window/Window.h"

class Input {
  public:
    static void Init(Window* window);
    static bool IsKeyPressed(int keycode);
    static bool IsMouseButtonPress(int button);
    static std::pair<float, float> GetMousePosition();
  private:
    static Window* s_Window;
};