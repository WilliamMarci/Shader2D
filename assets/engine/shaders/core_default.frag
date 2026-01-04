#version 330 core

in vec4 ourColor; // 改成 vec4
out vec4 FragColor;

void main()
{
    FragColor = ourColor;
}
