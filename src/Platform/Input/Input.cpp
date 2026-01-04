#include "Input.h"
#include <GLFW/glfw3.h>

Window* Input::s_Window = nullptr;

void Input::Init(Window* window) { s_Window = window; }

bool Input::IsKeyPressed(int keycode) {
    if (!s_Window) { return false; }
    auto window = static_cast<GLFWwindow*>(s_Window->GetNativeWindow());
    auto state = glfwGetKey(window, keycode);

    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPress(int button) {
    if (!s_Window) { return false; }

    auto window = static_cast<GLFWwindow*>(s_Window->GetNativeWindow());
    auto state = glfwGetMouseButton(window, button);
    
    return state == GLFW_PRESS;
}

std::pair<float, float> Input::GetMousePosition(){
    if (!s_Window) { return {0.0f, 0.0f}; }
    
    auto window = static_cast<GLFWwindow*>(s_Window->GetNativeWindow());
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    return {(float)xpos, (float)ypos};
}