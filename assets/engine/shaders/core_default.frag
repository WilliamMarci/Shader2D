#version 450 core

layout(location = 0) out vec4 color;

// 输入必须与顶点着色器匹配
layout(location = 0) in vec4 v_Color;
layout(location = 1) in vec2 v_TexCoord;

// flat 关键字必须匹配
layout(location = 2) flat in int v_TexIndex;

uniform sampler2D u_Textures[32];

void main() {
    vec4 texColor = v_Color;

    // 在 GLSL 450 中，通常可以直接使用变量索引数组
    // 只要你的显卡驱动支持 "Dynamic Indexing" (Intel Arc 肯定支持)
    if (v_TexIndex >= 0 && v_TexIndex < 32) {
         texColor *= texture(u_Textures[v_TexIndex], v_TexCoord);
    } else {
         // 越界保护，显示洋红色调试
         texColor = vec4(1.0, 0.0, 1.0, 1.0); 
    }

    color = texColor;
}