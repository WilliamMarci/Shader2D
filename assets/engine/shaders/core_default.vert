#version 330 core

// 输入：位置 (Location 0)
layout (location = 0) in vec3 aPos;
// 输入：颜色 (Location 1)
layout (location = 1) in vec3 aColor;

// 输出：传给片段着色器的颜色
out vec3 ourColor;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;
}
