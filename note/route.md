这份清单是根据我们之前的架构设计整理的**实战路线图**。你可以把它复制到你的笔记软件（如 Notion, Obsidian）中，每完成一项就打个钩。

---

# 🛠️ 独立游戏引擎开发路线清单 (Roadmap)

## 阶段一：基础设施与窗口 (Phase 1: Infrastructure)
**目标：** 搭建开发环境，跑通第一个黑窗口。
**预估时间：** 3-5 天

- [ ] **项目搭建**
    - [ ] 建立目录结构 (`src`, `assets`, `vendor`, `CMakeLists.txt`)。
    - [ ] 编写根目录 `CMakeLists.txt`。
    - [ ] 下载并集成第三方库：`GLFW`, `GLAD`, `GLM`。
- [ ] **窗口系统 (Platform Layer)**
    - [ ] 创建 `Window` 类，封装 GLFW 初始化与销毁。
    - [ ] 实现 `Window::Update()` 处理事件轮询 (`glfwPollEvents`)。
    - [ ] 成功运行并弹出一个空白窗口。
- [ ] **输入系统**
    - [ ] 封装 `Input` 类。
    - [ ] 实现 `Input::IsKeyPressed(KeyCode key)`。
    - [ ] 实现 `Input::GetMousePosition()`。
    - [ ] **测试：** 按下 ESC 键能关闭窗口。

## 阶段二：核心渲染器 (Phase 2: The "Raylib" Layer)
**目标：** 实现类似 Raylib 的简单绘图接口，画出图片。
**预估时间：** 2 周

- [ ] **OpenGL 基础封装**
    - [ ] 集成 `GLAD`，初始化 OpenGL 上下文。
    - [ ] 编写 `Shader` 类：读取文件、编译顶点/片段着色器、链接程序。
    - [ ] 编写 `Texture` 类：集成 `stb_image`，加载图片到 GPU。
- [ ] **批处理渲染器 (Batch Renderer) —— 🔥 核心难点**
    - [ ] 定义顶点结构体 `Vertex` (Pos, Color, TexCoord, TexID)。
    - [ ] 初始化渲染器：创建巨大的 VBO (顶点缓冲) 和 VAO。
    - [ ] 实现 `Renderer::BeginScene(Camera)` 和 `EndScene()`。
    - [ ] 实现 `Renderer::DrawQuad(pos, size, color)` (纯色)。
    - [ ] 实现 `Renderer::DrawQuad(pos, size, texture)` (贴图)。
    - [ ] 实现 `Flush()`：在 `EndScene` 或缓冲满时调用 `glDrawElements`。
- [ ] **摄像机系统**
    - [ ] 编写 `Camera` 类 (正交投影 Orthographic)。
    - [ ] 计算 View-Projection 矩阵并传给 Shader。
    - [ ] **测试：** 屏幕上画出 100 个方块，并能通过键盘移动摄像机查看它们。

## 阶段三：材质与资源管理 (Phase 3: Engine Core)
**目标：** 让资源加载更智能，支持自定义 Shader。
**预估时间：** 1 周

- [ ] **资源管理器**
    - [ ] 编写 `ResourceManager` (单例)。
    - [ ] 实现纹理缓存：如果 "wall.png" 加载过，直接返回指针，不重复加载。
    - [ ] 实现 Shader 库管理：`ShaderLibrary::Load("Core_PBR")`。
- [ ] **材质系统**
    - [ ] 编写 `Material` 类，持有 `Shader*` 和参数表。
    - [ ] 实现 `Material::SetColor()`, `Material::SetTexture()`。
    - [ ] 修改渲染器，使其支持提交带有不同 Material 的物体。
- [ ] **Shader 预处理器 (可选)**
    - [ ] 实现简单的字符串解析，支持 GLSL 中的 `#include` 语法。

## 阶段四：C/S 架构分离 (Phase 4: Architecture Split)
**目标：** 实现单人游戏的“伪联机”架构。
**预估时间：** 2 周

- [ ] **通用数据层**
    - [ ] 定义 `Packet` 结构体 (Type, Data)。
    - [ ] 定义 `PacketType` 枚举 (PlayerMove, WorldState)。
- [ ] **通信层 (Loopback)**
    - [ ] 编写线程安全的队列 `ThreadSafeQueue<Packet>`。
    - [ ] 实现 `LoopbackDriver`：Client `Push` -> Server `Pop`。
- [ ] **服务端 (Server)**
    - [ ] 创建 `GameServer` 类。
    - [ ] 编写服务端主循环 (Tick Loop)，固定 20 TPS。
    - [ ] 在独立线程 (`std::thread`) 中启动 Server。
- [ ] **客户端 (Client)**
    - [ ] 重构 `main.cpp`，主线程只负责 Client 逻辑。
    - [ ] 实现 `Client::SendInput()`：将键盘操作发给 Server。
    - [ ] 实现 `Client::OnPacketReceived()`：接收 Server 的坐标更新。
    - [ ] **测试：** Client 按 W，Server 收到并打印日志，Server 发回新坐标，Client 方块移动。

## 阶段五：游戏性基础 (Phase 5: Gameplay Foundation)
**目标：** 让方块动起来，有物理和 AI。
**预估时间：** 持续进行

- [ ] **物理系统**
    - [ ] 编写 `AABB` 类 (Axis-Aligned Bounding Box)。
    - [ ] 实现 `CheckCollision(AABB a, AABB b)`。
    - [ ] 在 Server 端进行移动前的碰撞检测。
- [ ] **AI 状态机**
    - [ ] 定义 `State` 基类 (Enter, Update, Exit)。
    - [ ] 编写 `StateMachine` 类。
    - [ ] 实现简单的怪物行为：`PatrolState` (巡逻) -> `ChaseState` (追逐)。
- [ ] **调试工具 (必做)**
    - [ ] 集成 `Dear ImGui`。
    - [ ] 制作一个调试面板，实时显示 FPS、实体数量、Server Tick 耗时。

## 阶段六：扩展与打磨 (Phase 6: Polish)
**目标：** 增加声音和 UI，使其像一个完整的游戏。
**预估时间：** 1 周

- [ ] **音频系统**
    - [ ] 集成 `miniaudio` 或 `SoLoud`。
    - [ ] 实现 `Audio::Play("bgm.mp3")`。
- [ ] **UI 系统**
    - [ ] 使用你的 `BatchRenderer` 绘制 UI 层 (HUD)。
    - [ ] 实现简单的文字渲染 (集成 `stb_truetype` 或使用 Bitmap Font)。

---

### 💡 关键里程碑检查点

1.  **里程碑 A:** 屏幕上出现一个带纹理的方块。 (完成阶段二)
2.  **里程碑 B:** 两个窗口（或调试模式下），一个控制方块移动，另一个能看到同步移动（虽然现在是单机，但逻辑上通过了 Server）。 (完成阶段四)
3.  **里程碑 C:** 一个怪物在巡逻，玩家靠近后怪物开始追逐。 (完成阶段五)