## 代码规范

# MyIndieProject 代码规范 (Code Style Guide)

## 1. 目录与文件结构

* **原则**：物理结构（文件夹）应严格对应逻辑结构（命名空间）。
* **头文件 (.h)**：必须包含 `#pragma once`。
* **包含顺序**：
  1. 对应的头文件 (如 `GameApp.cpp` 第一行是 `#include "GameApp.h"`)
  2. 本项目其他模块 (`#include "Platform/..."`)
  3. 第三方库 (`#include <glad/glad.h>`, `#include <glm/...>`)
  4. 标准库 (`#include <iostream>`, `#include <vector>`)

## 2. 命名约定 (Naming Convention)

| 类型                      | 规则            | 示例                              | 说明                 |
| :------------------------ | :-------------- | :-------------------------------- | :------------------- |
| **类/结构体**       | PascalCase      | `GameApp`, `BatchRenderer`    | 大驼峰               |
| **函数/方法**       | PascalCase      | `Update()`, `CompileShader()` | 大驼峰，动词开头     |
| **变量 (局部)**     | camelCase       | `vertexSource`, `windowWidth` | 小驼峰               |
| **变量 (成员)**     | m_PascalCase    | `m_RendererID`, `m_Window`    | `m_` 前缀 + 大驼峰 |
| **变量 (静态成员)** | s_PascalCase    | `s_Instance`                    | `s_` 前缀          |
| **常量/宏**         | SCREAMING_SNAKE | `MAX_SPRITES`, `PI`           | 全大写 + 下划线      |
| **命名空间**        | PascalCase      | `MyEngine`, `Client`          | 避免缩写             |

## 3. 现代 C++ 特性 (Modern C++ Usage)

我们强制使用 **C++17** 标准。

* **内存管理**：

  * **严禁**在业务逻辑中使用裸指针 (`new`/`delete`)。
  * **必须**使用智能指针：
    * 独占资源用 `std::unique_ptr` (如 `m_Window`, `m_Shader`)。
    * 共享资源用 `std::shared_ptr` (如未来的 `Texture`, `Material`)。
  * *例外*：底层封装（如 `Window` 内部调用 GLFW）或观察者模式（传递 `Window*` 给 Input）可以使用裸指针，但不负责销毁。
* **关键字**：

  * 虚函数重写必须加 `override`。
  * 不修改成员变量的函数必须加 `const` (如 `IsKeyPressed`, `GetWidth`)。
  * 构造函数若只有一个参数，尽量加 `explicit` 防止隐式转换。

## 4. 类设计规范 (Class Design)

* **RAII (资源获取即初始化)**：

  * 类的构造函数负责申请资源（如 `glGenBuffers`）。
  * 类的析构函数负责释放资源（如 `glDeleteBuffers`）。
  * 禁止出现资源泄漏。
* **Pimpl (可选)**：对于依赖复杂的底层实现（如包含大量 OpenGL 头文件），尽量在 `.cpp` 中实现细节，保持 `.h` 干净。

## 5. 渲染与数学

* **数学库**：统一使用 **GLM**。
  * 向量：`glm::vec2`, `glm::vec3`, `glm::vec4`
  * 矩阵：`glm::mat4`
* **OpenGL 调用**：
  * 禁止在 `GameApp` 等高层逻辑中直接调用 `glDrawArrays` 等原生 API。
  * 必须通过 `Renderer` 或 `Shader` 等封装类进行调用。

## 6. 注释与文档

* **头文件注释**：解释“这个类是干什么的”。
* **方法注释**：如果逻辑复杂，解释“为什么这么做”，而不是“做了什么”（代码本身就是解释）。
* **TODO**：使用 `// TODO: 说明` 标记待办事项。

---

### 示例代码片段

```cpp
// src/Platform/OpenGL/Texture.h

#pragma once

#include <string>
#include <memory>

// 命名空间（可选，视项目规模定）
namespace MyEngine {

    class Texture {
    public:
        // 构造函数：加载纹理
        explicit Texture(const std::string& path);
        ~Texture();

        // 绑定到指定槽位
        void Bind(unsigned int slot = 0) const;

        // Getters
        int GetWidth() const { return m_Width; }
        int GetHeight() const { return m_Height; }

    private:
        unsigned int m_RendererID;
        std::string m_FilePath;
        int m_Width, m_Height, m_BPP; // BPP = Bytes Per Pixel
    };

}
```

记录好这份规范后，我们就可以在接下来的 `BatchRenderer` 开发中严格执行它，写出漂亮、健壮的代码了。

MyIndieProject/
├── CMakeLists.txt                  # 顶层构建脚本
├── assets/                         # 【资源目录】
│   ├── engine/                     # >> 引擎内置资源 (不可删除)
│   │   ├── shaders/                # 核心着色器库
│   │   │   ├── include/            # GLSL 头文件 (被其他 shader include)
│   │   │   │   ├── common.glsl     # 通用数学函数
│   │   │   │   └── lighting.glsl   # 光照计算公式
│   │   │   ├── core_sprite.vs      # 2D 精灵顶点着色器
│   │   │   ├── core_sprite.fs      # 2D 精灵片段着色器
│   │   │   ├── core_pbr.vs         # 3D 标准物理渲染顶点
│   │   │   ├── core_pbr.fs         # 3D 标准物理渲染片段
│   │   │   └── core_ui.fs          # UI 渲染专用
│   │   └── textures/               # 默认纹理 (如纯白图片、缺失材质紫黑格)
│   └── game/                       # >> 具体游戏资源
│       ├── textures/
│       └── shaders/                # 游戏特有的特殊着色器 (可选)
├── src/
│   ├── Common/                     # 【公共数据层】
│   │   ├── DataTypes.h             # u8, u32, f32 等类型定义
│   │   └── Math/                   # 向量、矩阵库封装
│   ├── Engine/                     # 【引擎核心层】(编译为静态库 EngineLib)
│   │   ├── Core/
│   │   │   ├── Application.h       # 引擎生命周期管理
│   │   │   ├── Logger.h            # 日志
│   │   │   └── FileSystem.h        # 跨平台文件读取
│   │   ├── Render/                 # >> 渲染核心 <<
│   │   │   ├── Backend/            # OpenGL/Vulkan API 隔离层
│   │   │   │   ├── OpenGLBuffer.cpp
│   │   │   │   └── OpenGLTexture.cpp
│   │   │   ├── Shader/             # >> 着色器系统 <<
│   │   │   │   ├── Shader.h        # 单个 Shader Program 的封装
│   │   │   │   ├── ShaderLibrary.h # Shader 管理器 (单例)
│   │   │   │   └── ShaderParser.h  # 解析 #include 等预处理指令
│   │   │   ├── Material/           # >> 材质系统 <<
│   │   │   │   ├── Material.h      # 材质基类 (持有 Shader 引用)
│   │   │   │   └── MaterialInstance.h # 材质实例 (持有具体参数)
│   │   │   └── Renderer2D.h        # 2D 批处理渲染器
│   │   └── Platform/               # 窗口与输入 (GLFW)
│   ├── Server/                     # 【服务端逻辑】
│   │   └── ... (同前文)
│   └── Client/                     # 【客户端表现】
│       ├── GameApp.cpp             # 客户端入口
│       ├── Gameplay/               # 游戏逻辑
│       │   ├── PlayerController.cpp
│       │   └── LevelRenderer.cpp   # 关卡渲染
│       └── Resources/              # 客户端资源句柄
└── vendor/                         # 第三方库 (GLAD, GLFW, GLM, stb_image)

工作量评估
假设你是一名有一定 C++ 基础的开发者，每天投入 2-3 小时：

基础搭建 (Window + OpenGL Context): 1-2 天
资源加载 (Texture/Shader/Audio): 3-5 天
核心渲染器 (Batch Renderer - 关键): 1-2 周
文字渲染 (Font): 1 周 (这是最繁琐的部分之一)
输入与基础UI: 3-5 天
调试与跨平台适配: 持续进行
总计：约 1 到 1.5 个月，你就能拥有一个属于自己的、能跑通游戏逻辑的“微型引擎”。

如何实现？（核心路线图）
Raylib 的核心魔力在于它把复杂的 OpenGL 状态机封装成了简单的立即模式 (Immediate Mode) 函数（如 DrawTexture）。你需要做的就是实现这一层封装。

我们需要站在巨人的肩膀上，不要从零写驱动代码。请使用以下 "黄金四件套"：

GLFW: 搞定窗口和输入 (Windows/Linux)。
GLAD: 加载 OpenGL 函数指针。
stb_image: 加载 png/jpg (单头文件)。
glm: 数学库。

MyIndieGame/
├── src/
│   ├── Platform/           # 【底层封装】(你的 Raylib 替代品)
│   │   ├── Window.cpp      # GLFW 封装
│   │   ├── Input.cpp       # 键盘鼠标
│   │   ├── OpenGL/         # 图形 API
│   │   │   ├── Shader.cpp
│   │   │   ├── Texture.cpp
│   │   │   └── BatchRenderer.cpp # 核心！
│   │   └── Audio/          # MiniAudio 封装
│   │
│   ├── Engine/             # 【中间层】
│   │   ├── ResourceManager.h
│   │   ├── Camera.h
│   │   └── Timer.h
│   │
│   └── Game/               # 【游戏逻辑】(这里写起来就像用 Raylib 一样)
│       ├── Player.cpp
│       └── World.cpp

如果你采纳 Raylib，你的架构压力会瞬间减小 80%。你不再需要写 Engine/Render/Backend，也不用管 Engine/Platform

太棒了！Linux 是开发底层系统和游戏引擎的绝佳环境（也是服务器部署的目标环境）。在 Linux 下开发 C++ 游戏引擎通常比 Windows 更顺滑，因为工具链（Toolchain）更统一。

这是针对 **Linux (Ubuntu/Debian/Arch)** 的详细清单。

---

### 📦 第一部分：系统包安装清单 (System Packages)

在 Linux 上，你不需要去各个官网下载安装包，直接用包管理器（apt/pacman）安装即可。

打开终端，运行以下命令（以 Ubuntu/Debian 为例）：

#### 1. 基础编译工具链

```bash
sudo apt update
sudo apt install build-essential gdb cmake git
```

* **build-essential:** 包含了 `gcc`, `g++`, `make` 等核心编译工具。
* **gdb:** 调试器。
* **cmake:** 构建系统。

#### 2. 图形库依赖 (GLFW 依赖)

GLFW 在 Linux 上需要依赖 X11 或 Wayland 的开发库。

```bash
sudo apt install libglfw3-dev
```

* **libglfw3-dev:** 这会直接安装 GLFW 的头文件和静态库。你甚至不需要自己下载源码编译它（虽然自己编译更灵活，但初期直接用 apt 安装最省事）。
* *如果后续编译报错缺库，可能还需要安装这些 X11 相关的库：*

  ```bash
  sudo apt install libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
  ```

#### 3. IDE / 编辑器

* **推荐：** **VS Code** (轻量，插件丰富)
  * 安装插件：`C/C++` (Microsoft), `CMake Tools` (Microsoft)。
* **硬核推荐：** **CLion** (JetBrains)
  * 收费（学生免费），但对 CMake 和 C++ 的支持是业界最强的。
* **极简推荐：** **Vim / Neovim** (如果你习惯终端操作)。

---

### 📚 第二部分：项目库清单 (Vendor Libraries)

和 Windows 一样，这些库建议以源码形式放在你的 `vendor/` 目录下，或者使用 Git Submodule。

1. **GLAD:** (必须手动生成)

   * 去 [glad.dav1d.de](https://glad.dav1d.de/) 生成并下载 `glad.c` 和头文件。
   * 放到 `vendor/glad/`。
2. **GLM:** (Header-only)

   * `sudo apt install libglm-dev` (可以直接装系统里)
   * 或者下载源码放到 `vendor/glm/` (推荐，保证版本可控)。
3. **stb_image:** (Header-only)

   * 下载 `stb_image.h` 放到 `vendor/stb/`。

---

### 🛠️ 第三部分：CMake 配置 (Linux 特有)

在 Linux 下，你的 `CMakeLists.txt` 链接库的方式会稍微简单一点，因为系统路径里通常已经有了。

**示例 `CMakeLists.txt` 片段：**

```cmake
cmake_minimum_required(VERSION 3.10)
project(MyIndieGame)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)

# 1. 查找系统里的 GLFW (如果是 apt 安装的)
find_package(glfw3 3.3 REQUIRED)

# 2. 包含 GLAD 和 GLM (假设在 vendor 下)
include_directories(vendor/glad/include)
include_directories(vendor/glm)
include_directories(vendor/stb)

# 3. 添加 GLAD 的源文件
add_library(glad vendor/glad/src/glad.c)

# 4. 你的游戏可执行文件
add_executable(Game src/main.cpp src/Window.cpp ...)

# 5. 链接库
# Linux 下必须显式链接 OpenGL 库 (GL), 线程库 (pthread), 时间库 (dl)
target_link_libraries(Game 
    PRIVATE 
    glad 
    glfw 
    GL          # 系统 OpenGL 驱动
    dl          # 动态链接加载器
    pthread     # 多线程支持 (C/S架构必须)
)
```

---

### 🧠 第四部分：Linux 下的学习重点

除了之前提到的 OpenGL 和 C++，在 Linux 下开发你需要额外了解：

1. **Make / Ninja:**

   * CMake 生成的是 `Makefile`。你需要知道在终端敲 `make` 来编译。
   * 或者用 `ninja` (速度更快)：`sudo apt install ninja-build`，然后 `cmake -G Ninja ..`。
2. **调试 (GDB / VS Code):**

   * 在 Linux 下没有 Visual Studio 那种傻瓜式调试器。
   * 你需要学会配置 VS Code 的 `.vscode/launch.json` 来启动 GDB 调试你的程序。
3. **显卡驱动:**

   * 确认你的 Linux 显卡驱动装好了。运行 `glxinfo | grep "OpenGL version"` 看看是不是显示了正确的版本（比如 4.6），而不是 Mesa 软件渲染。

### 🚀 你的第一步 (Linux 版)

1. 打开终端，安装 `build-essential`, `cmake`, `libglfw3-dev`。
2. 创建一个文件夹 `MyGame`。
3. 创建一个 `main.cpp`，写一个简单的 Hello World。
4. 创建一个 `CMakeLists.txt`。
5. 运行：
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ./MyGame
   ```
6. 如果打印出 Hello World，环境就通了！接下来引入 GLFW 画窗口。


![1767510968972](image/mem/1767510968972.png)

这张图展示了从你的 C++ 代码到屏幕显示的完整流程。请注意**绿色**的部分是你可以写代码控制的（Shader），**灰色**的部分是显卡硬件自动完成的。

**总结：**
你写的 GLSL -> 传给 OpenGL API -> 显卡驱动接收 -> **驱动内置编译器编译** -> GPU 机器码 -> 在显卡上运行

GLSL (OpenGL Shading Language) 的语法 **非常像 C 语言** ，但它是专门为数学计算和图形处理“魔改”过的。

**核心特点：**

1. **强类型：** 必须声明类型，不能随便混用（比如 `int` 不能直接赋值给 `float`，必须强转）。
2. **内置数学类型：** 这是它最强大的地方。它原生支持向量和矩阵运算。
   * `vec2`, `vec3`, `vec4`: 2/3/4维向量（比如坐标、颜色）。
   * `mat2`, `mat3`, `mat4`: 2x2, 3x3, 4x4 矩阵。
3. **并行思维：** 没有 `printf`，没有 `cin`。你的代码是同时在几千个核心上跑的。

```glsl
// --- 变量定义 ---
int  i = 10;
float f = 3.14;
bool b = true;

// --- 向量 (Vector) ---
vec3 pos = vec3(1.0, 2.0, 3.0);
vec3 color = vec3(1.0, 0.0, 0.0); // 红色

// 向量极其灵活的访问方式 (Swizzling)
float x = pos.x;      // 取第1个分量
vec2 xy = pos.xy;     // 取前2个分量，变成一个 vec2
vec3 c = color.bgr;   // 甚至可以乱序取！变成 (0, 0, 1) 蓝色

// --- 矩阵 (Matrix) ---
mat4 projection; // 4x4 投影矩阵

// --- 输入输出修饰符 ---
in  vec3 vPos;   // 输入：从上一阶段传进来的
out vec4 fColor; // 输出：传给下一阶段的
uniform float uTime; // 全局变量：CPU 传进来的，对所有像素都一样

// --- 函数 ---
// 有 main 函数作为入口
void main() {
    // 内置数学函数极多
    float d = distance(pos, vec3(0.0)); // 计算距离
    float s = sin(uTime);               // 正弦
    vec3 n = normalize(pos);            // 归一化
}

```
