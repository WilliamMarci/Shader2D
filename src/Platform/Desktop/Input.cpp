#include "Engine/Input/Input.h"
// 移除 #include "Engine/Core/Application.h" 以解耦
#include <GLFW/glfw3.h>

namespace Engine {

    static GLFWwindow* s_NativeWindow = nullptr;

    void Input_SetContext(void* window) {
        s_NativeWindow = static_cast<GLFWwindow*>(window);
    }

    bool Input::IsKeyPressed(KeyCode keycode) {
        if (!s_NativeWindow) return false;
        auto state = glfwGetKey(s_NativeWindow, static_cast<int32_t>(keycode));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::IsMouseButtonPressed(MouseCode button) {
        if (!s_NativeWindow) return false;
        auto state = glfwGetMouseButton(s_NativeWindow, static_cast<int32_t>(button));
        return state == GLFW_PRESS;
    }

    std::pair<float, float> Input::GetMousePosition() {
        if (!s_NativeWindow) return { 0.0f, 0.0f };
        double xpos, ypos;
        glfwGetCursorPos(s_NativeWindow, &xpos, &ypos);
        return { (float)xpos, (float)ypos };
    }

    float Input::GetMouseX() {
        auto [x, y] = GetMousePosition();
        return x;
    }

    float Input::GetMouseY() {
        auto [x, y] = GetMousePosition();
        return y;
    }

}
