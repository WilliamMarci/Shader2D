这份文档总结了我们讨论的所有核心架构设计。它旨在指导你从零构建一个**基于 C/S 架构、支持联机扩展、拥有自定义渲染管线的轻量级 C++ 游戏引擎**。

---

# 独立游戏引擎架构设计文档 (Custom C++ Game Engine)

## 1. 项目愿景与核心理念
*   **架构模式：** **Client-Server (C/S)**。即使是单人模式，本质上也是客户端连接到一个内部运行的本地服务器（Local Server）。这保证了未来联机功能的无缝扩展。
*   **技术路线：** 不依赖庞大的商业引擎，而是构建一个**类似 Raylib 的微型框架**。
*   **核心依赖：** GLFW (窗口), GLAD (OpenGL), GLM (数学), stb_image (图像), MiniAudio (音频)。

---

## 2. 详细目录结构
该结构实现了**引擎与游戏分离**，以及**服务端与客户端分离**。

```text
MyIndieProject/
├── CMakeLists.txt                  # 顶层构建脚本
├── assets/                         # 【资源仓库】
│   ├── engine/                     # >> 引擎核心资源 (只读)
│   │   ├── shaders/                # 核心着色器 (PBR, Sprite, UI)
│   │   │   ├── include/            # GLSL 头文件 (供 #include 使用)
│   │   │   └── core_default.glsl
│   │   └── textures/               # 缺省纹理 (Error Texture)
│   └── game/                       # >> 具体游戏资源
│       ├── textures/
│       ├── audio/
│       └── data/                   # JSON 配置/存档
├── src/
│   ├── Common/                     # 【公共层】(双端共享)
│   │   ├── DataTypes.h             # 基础类型 (u8, i32, Vec3)
│   │   ├── Protocol/               # 网络协议 (Packet 定义)
│   │   └── Math/                   # 碰撞盒 (AABB), 数学工具
│   ├── Platform/                   # 【底层抽象层】(类似 Raylib 的实现)
│   │   ├── Window/                 # GLFW 窗口封装
│   │   ├── OpenGL/                 # 渲染后端
│   │   │   ├── Shader.cpp          # 编译与链接
│   │   │   ├── Texture.cpp         # 纹理上传
│   │   │   └── BatchRenderer.cpp   # 核心：批处理渲染器
│   │   └── Audio/                  # 音频后端
│   ├── Engine/                     # 【引擎中间层】
│   │   ├── Resources/              # 资源管理器 (缓存池)
│   │   ├── Render/                 # 材质系统 (Material System)
│   │   └── Core/                   # 日志, 文件IO, 计时器
│   ├── Server/                     # 【服务端逻辑】(权威逻辑)
│   │   ├── GameServer.cpp          # 主循环 (Tick Loop)
│   │   ├── World/                  # 物理世界模拟
│   │   ├── AI/                     # 状态机与行为逻辑
│   │   └── Network/                # 接收指令/广播状态
│   └── Client/                     # 【客户端表现】(渲染与输入)
│       ├── GameApp.cpp             # 主循环 (Frame Loop)
│       ├── Input/                  # 输入采集 -> 转换为 Packet
│       ├── View/                   # 渲染逻辑 (调用 Engine 绘制)
│       └── Network/                # 预测与插值 (Prediction/Interpolation)
└── vendor/                         # 第三方库源码
```

---

## 3. 核心模块实现方案

### A. 渲染与着色器系统 (Material System)
为了让客户端开发方便，我们将 Shader（代码）与 Material（参数）分离。

1.  **Shader (引擎层):**
    *   支持 `#include` 预处理（需编写 `ShaderParser`）。
    *   引擎启动时自动加载 `assets/engine/shaders` 下的所有 Shader 到 `ShaderLibrary`。
2.  **Material (应用层):**
    *   客户端创建 `Material` 实例，引用一个 Shader。
    *   提供简单接口：`mat->SetColor("u_Color", red);`，无需编写 GLSL。
3.  **批处理 (Batching):**
    *   不要对每个物体调用 `glDrawArrays`。
    *   实现 `Renderer2D::DrawQuad(...)`，将顶点数据写入一个大 Buffer，帧末统一提交。

### B. Client-Server 架构细节
*   **单人模式 (Internal Server):**
    *   主线程运行 Client Loop (渲染)。
    *   子线程运行 Server Loop (逻辑)。
    *   **通信：** 使用线程安全的**内存队列 (Memory Queue)**，直接传递数据包指针，零延迟。
*   **联机模式 (Dedicated Server):**
    *   Server 运行在独立进程。
    *   **通信：** 使用 UDP (ENet) 进行序列化传输。
*   **同步策略：**
    *   Server 固定频率 (如 20 TPS) 更新逻辑。
    *   Client 高帧率 (60+ FPS) 渲染，利用**插值 (Interpolation)** 平滑显示 Server 发来的坐标。

### C. AI 与状态机 (FSM)
实现“大脑”与“躯体”分离。

*   **State 接口:** `Enter()`, `Update()`, `Exit()`。
*   **StateMachine:** 持有 `CurrentState`，负责切换。
*   **Sensor 接口:** `CanSee(target)`, `CanHear(target)`，避免 AI 直接读取全图数据。

### D. 文件 IO 与 存档
*   使用 C++17 `<filesystem>` 处理路径。
*   使用 `nlohmann/json` 进行序列化。
*   **存档逻辑：** 只由 Server 负责保存世界状态（玩家位置、背包、怪物状态）。

### E. UI 系统
*   **工具/调试 UI:** 集成 **Dear ImGui** (必选，极大提高开发效率)。
*   **游戏内 HUD:** 使用自己的批处理渲染器绘制带纹理的矩形（血条、图标）。

---

## 4. 开发路线图 (Roadmap)

如果你决定自己实现类似 Raylib 的微型框架，建议按此顺序进行：

### 第一阶段：地基 (预计 1-2 周)
1.  **窗口:** 配置 CMake，集成 GLFW，弹出一个黑窗口。
2.  **输入:** 封装 `Input::IsKeyPressed(Key::A)`。
3.  **上下文:** 集成 GLAD，设置 OpenGL 3.3 Core Profile。

### 第二阶段：渲染核心 (预计 2-3 周)
1.  **Shader 类:** 读取文件、编译、报错输出。
2.  **纹理类:** 集成 stb_image，加载图片到 GPU。
3.  **批处理渲染器 (MVP):** 实现 `BatchRenderer::DrawQuad()`，能画出 1000 个旋转的方块而不卡顿。

### 第三阶段：架构整合 (预计 2 周)
1.  **C/S 分离:** 拆分 `main.cpp`，建立 Server 线程和 Client 主循环。
2.  **通信:** 实现内存队列，Client 发送 "Move"，Server 打印日志。
3.  **同步:** Server 更新坐标发回 Client，Client 渲染移动的方块。

### 第四阶段：游戏性填充 (持续)
1.  **物理:** 添加简单的 AABB 碰撞检测。
2.  **AI:** 引入状态机，让方块自动巡逻。
3.  **音频:** 集成 MiniAudio 播放背景音乐。

---

## 5. 关键代码片段速查

**1. 状态机接口:**
```cpp
class State {
public:
    virtual void Enter(GameObject* owner) = 0;
    virtual void Update(GameObject* owner, float dt) = 0;
    virtual void Exit(GameObject* owner) = 0;
};
```

**2. 抽象网络接口:**
```cpp
class INetworkDriver {
public:
    virtual void Send(const Packet& packet) = 0;
    virtual bool Poll(Packet& outPacket) = 0;
};
// 实现 LoopbackDriver (单机) 和 ENetDriver (联机)
```

**3. 材质使用:**
```cpp
// 客户端代码
auto* shader = ShaderLibrary::Get("Core_PBR");
Material* mat = new Material(shader);
mat->SetTexture("u_Albedo", grassTexture);
Renderer::Submit(mesh, mat, transform);
```

---

这份文档是你项目的**蓝图**。当你迷失方向或不知道下一步该做什么时，请随时查阅此文档，或直接问我具体的实现细节！祝开发顺利！