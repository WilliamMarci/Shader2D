#version 330 core

// 输入：从顶点着色器传来的颜色
in vec3 ourColor;

// 输出：最终屏幕颜色
out vec4 FragColor;

void main()
{
    FragColor = vec4(ourColor, 1.0);
}
