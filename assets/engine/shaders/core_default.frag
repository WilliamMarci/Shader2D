#version 450 core

layout(location = 0) out vec4 o_Color;

in VS_OUT {
    vec4 Color;
    vec2 TexCoord;
    flat float TexIndex; // 必须匹配 vertex shader 的 flat
    float TilingFactor;
} fs_in;

// 绑定 32 个纹理槽
// 在 OpenGL 4.5 中，我们可以直接索引这个数组
layout(binding = 0) uniform sampler2D u_Textures[32];

void main() {
    vec4 texColor = fs_in.Color;
    vec2 tiledCoords = fs_in.TexCoord * fs_in.TilingFactor;

    // 加上 0.5 避免浮点精度问题导致的索引偏移 (例如 4.999 -> 4)
    int index = int(fs_in.TexIndex + 0.5);

    // 动态索引采样
    // 注意：如果 index 超出范围，行为是未定义的，但在我们的 C++ 代码里控制了最大值
    texColor *= texture(u_Textures[index], tiledCoords);

    // Alpha Cutoff
    if (texColor.a < 0.01)
        discard;

    o_Color = texColor;
}