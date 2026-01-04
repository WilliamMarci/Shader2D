#pragma once

#include <functional>
#include <string>

struct GLFWwindow;

class Window {
  public:
    struct WindowData {
        std::string Title;
        unsigned int Width, Height;
        bool VSync;
    };

    Window(const std::string& title, unsigned int width, unsigned int height);
    ~Window();

    void Update();
    bool ShouldClose() const;
    GLFWwindow* GetNativeWindow() const { return m_Window; }
    void Close();

    unsigned int GetWidth() const { return m_Data.Width; }
    unsigned int GetHeight() const { return m_Data.Height; }

  private:
    void Init();
    void Shutdown();

  private:
    GLFWwindow* m_Window;
    WindowData m_Data;
};