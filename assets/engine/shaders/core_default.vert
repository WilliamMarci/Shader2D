#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;

uniform mat4 u_ViewProjection;

// 使用 Interface Block 传递数据到 Fragment Shader，更整洁
out VS_OUT {
    vec4 Color;
    vec2 TexCoord;
    flat float TexIndex; // flat: 禁止插值，这对索引至关重要！
    float TilingFactor;
} vs_out;

void main() {
    vs_out.Color = a_Color;
    vs_out.TexCoord = a_TexCoord;
    vs_out.TexIndex = a_TexIndex;
    vs_out.TilingFactor = a_TilingFactor;
    
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}