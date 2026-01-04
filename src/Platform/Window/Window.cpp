#include "Window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

Window::Window(const std::string& title, unsigned int width, unsigned int height) : m_Data{title, width, height, true} {
    Init();
}

Window::~Window() { Shutdown(); }

void Window::Init() {
    if (!glfwInit()) {
        std::cerr << "[Window] Failed to initialize GLFW" << std::endl;
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // MacOS specific hint
#endif

    m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
    if (!m_Window) {
        std::cerr << "[Window] Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_Window);

    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (!status) {
        std::cerr << "[Window] Failed to initialize GLAD" << std::endl;
        return;
    }

    std::cout << "[Window] OpenGL Info" << std::endl;
    std::cout << "  Vendor:     " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "  Renderer:   " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "  Version:    " << glGetString(GL_VERSION) << std::endl;

    glViewport(0, 0, m_Data.Width, m_Data.Height);
    glfwSetWindowUserPointer(m_Window, &m_Data);
}

void Window::Shutdown() {
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void Window::Update() {
    glfwSwapBuffers(m_Window);
    glfwPollEvents();
}
void Window::Close() { glfwSetWindowShouldClose(m_Window, true); }

bool Window::ShouldClose() const { return glfwWindowShouldClose(m_Window); }