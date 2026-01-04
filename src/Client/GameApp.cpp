#include "GameApp.h"
#include "Platform/Input/Input.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

GameApp::GameApp() {

    m_Window = std::make_unique<Window>("My Indie Engine", 800, 600);


    Input::Init(m_Window.get());

    float vertices[] = {
        // 位置              // 颜色
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // 左下 (红)
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // 右下 (绿)
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f  // 顶部 (蓝)
    };
        // ---------------------------------------------
    // 2. 创建 OpenGL 缓冲 (VAO, VBO)
    // ---------------------------------------------
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    // 绑定 VAO (接下来的配置都属于这个 VAO)
    glBindVertexArray(m_VAO);

    // 绑定 VBO 并上传数据
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 告诉 OpenGL 怎么解析数据
    // 属性 0: 位置 (3个 float, 步长 6*float, 偏移 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 属性 1: 颜色 (3个 float, 步长 6*float, 偏移 3*float)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 解绑
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

    // ---------------------------------------------
    // 3. 加载 Shader
    // ---------------------------------------------
    // 注意路径：相对于可执行文件的路径，或者绝对路径
    // 我们假设在项目根目录运行，或者 build/bin 下运行，需要根据实际情况调整
    // 建议：暂时写死相对路径，稍后我们做资源管理器
    m_Shader = std::make_unique<Shader>(
        "assets/engine/shaders/core_default.vert",
        "assets/engine/shaders/core_default.frag"
    );

}

GameApp::~GameApp() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}

void GameApp::Run() {
    while (m_Running && !m_Window->ShouldClose()) {
        Update();
        Render();
        m_Window->Update();
    }
}

void GameApp::Update() {

    if (Input::IsKeyPressed(GLFW_KEY_ESCAPE)) {
        m_Running = false;
    }

    if (Input::IsKeyPressed(GLFW_KEY_A)) {
        std::cout << "GameApp: A key pressed!" << std::endl;
    }
}

void GameApp::Render() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

        // 1. 激活 Shader
    m_Shader->Bind();

    // 2. 绑定 VAO
    glBindVertexArray(m_VAO);

    // 3. 绘制三角形 (3个顶点)
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
