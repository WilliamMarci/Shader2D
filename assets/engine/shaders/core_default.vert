#version 450 core

// 顶点属性输入
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;

uniform mat4 u_ViewProjection;

// 输出到片段着色器
// layout(location = x) 在 450 版本是合法的
layout(location = 0) out vec4 v_Color;
layout(location = 1) out vec2 v_TexCoord;

// flat 关键字：禁止插值，必须传 int
layout(location = 2) flat out int v_TexIndex;

void main() {
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    
    // 强制转换 float -> int
    v_TexIndex = int(a_TexIndex);
    
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}