# 🗺️ 独立游戏引擎开发路线图 (Master Roadmap)

这份清单涵盖了从零开始到实现一个具有 2D 光追、RTS 能力和伪联机架构引擎的完整路径。

## ✅ 阶段一：基础设施 (Phase 1: Infrastructure)
**状态：已完成**
- [x] **项目搭建**: CMake, 目录结构, 第三方库 (GLFW, GLAD, GLM).
- [x] **窗口系统**: `Window` 类封装, 事件轮询.
- [x] **输入系统**: `Input` 类, 键盘/鼠标状态获取.
- [x] **OpenGL 上下文**: GLAD 初始化, 错误回调.

## 🚧 阶段二：渲染核心与重构 (Phase 2: Renderer Core & Refactor)
**目标：** 建立稳健、可扩展的 2D 渲染管线。消除重复代码，支持图集与动画。
**当前状态：进行中**

- [x] **Shader 系统**: 加载、编译、Uniform 缓存.
- [x] **摄像机系统**: `OrthographicCamera`, Zoom (缩放), View-Projection 矩阵.
- [x] **基础批处理**: `BatchRenderer` 架构, 巨大的 VBO, 基础 Quad 绘制.
- [x] **CPU 剔除**: AABB 视锥剔除 (Culling).
- [ ] **渲染器重构 (Renderer Refactoring)** 🔥 *(New)*
  - [ ] **统一提交接口**: 实现私有函数 `DrawQuadInternal`，收拢所有写 Buffer 的逻辑.
  - [ ] **清理公开 API**: 让所有 `DrawQuad` 重载版本都调用 `DrawQuadInternal`，避免修改一处动全身.
  - [ ] **扩展顶点结构**: 规范化 `QuadVertex` 结构体，为未来添加 Normal/EntityID 预留空间.
- [ ] **高级纹理与动画 (Advanced Texture & Animation)** 🔥 *(New)*
  - [ ] 集成 `stb_image` 与 `Texture2D` 类封装.
  - [ ] **子纹理系统 (`SubTexture2D`)**: 实现基于 UV 坐标切割纹理，支持 Sprite Sheet (图集).
  - [ ] **精灵组件 (`Sprite` & `SpriteAnimation`)**: 
    - [ ] 创建 `Sprite` 类 (持有 SubTexture, Color, Tiling).
    - [ ] 创建 `SpriteAnimation` 类 (管理帧序列、播放速度、当前 UV).
- [ ] **混合与旋转**: `glBlendFunc` (透明度) 和 Z 轴旋转支持.

### Additional Notes

- [x] 添加LOG系统和宏定义，方便调试和日志记录。
- [x] Event机制，重构现有的事件处理系统，使其更灵活和易于扩展。
- [ ] 设置Math库，统一使用GLM进行向量和矩阵运算， 向引擎用户暴露常用类型如`vec2`, `vec3`, `mat4`等，不必每次都引用glm.h.

## 🎨 阶段三：材质与资源管理 (Phase 3: Material & Resources)
**目标：** 建立统一的资源管线，为未来的“风格化渲染”做准备。我们需要像处理 3D 材质一样处理 2D 精灵。

- [ ] **资源管理器 (Resource Manager)**
  - [ ] 单例模式 `ResourceManager`.
  - [ ] **缓存机制**: 避免重复加载同一个纹理/Shader (使用 `std::unordered_map`).
  - [ ] **异步加载原型**: 预留接口，未来在独立线程加载大资源。
- [ ] **现代材质系统 (Modern Material System)** 🔥 *(Updated)*
  - [ ] **材质基类 (`Material`)**: 
    - [ ] 这是一个数据容器，持有 `Shader` 引用和参数表 (Uniform Buffer).
    - [ ] 允许设置 `AlbedoMap`, `NormalMap`, `Roughness` 等参数.
  - [ ] **渲染器适配**: 修改 `Renderer2D`，使其能根据 `Material->GetShaderID()` 进行批处理分类 (Sorting & Batching).
  - [ ] **材质实例 (Material Instance)**: 允许不同物体共用一个 Shader 但参数不同 (如红色塑料 vs 蓝色塑料).
- [ ] **Shader 库**: 内置常用 Shader (`Unlit`, `Standard2D`, `PixelArt`).
  - [ ] 统一规范：标准 uniform/attribute 列表（MVP / Albedo / Normal / Roughness / UV）。
  - [ ] Shader 热重载：检测文件变化并重新编译替换运行时 Shader。
- [ ] **模型导入器 (Model Importer)**
  - [ ] 集成 Assimp：引入并封装一个简单的导入接口，支持 .obj、.gltf 等常见格式。
  - [ ] Mesh 类：解析顶点属性（Position, Normal, UV，可选 Tangent/Bitangent），生成并管理 VAO / VBO / IBO。
  - [ ] SubMesh 支持：按材质分割索引缓冲，支持一个模型包含多个材质的情况。
  - [ ] 材质与纹理映射：从 Assimp 材质读取纹理路径，交由 ResourceManager 加载 Texture2D（支持 Albedo/Normal/Emission）。
  - [ ] API 设计：提供同步与异步加载接口，加载时返回占位网格/纹理以避免阻塞渲染线程。
  - [ ] 测试用例：成功加载并渲染 Blender 导出的 Suzanne（.obj 或 .gltf），验证顶点法线、UV、子网格与材质正确显示；支持切换线框/填充模式。
  - [ ] 错误与兼容性处理：日志提示缺失属性或纹理，自动回退（如无法读入法线则启用法线重计算或缺省法线）。
  - [ ] 性能注意：支持静态网格合批（合并相同材质子网格）、延迟/分块上传大网格，留出异步加载扩展点。

## 阶段3.5 3D 风格化与混合渲染 (Phase 3.5: Stylized 3D)
目标： 在 2D 世界中引入 3D 物体，并赋予其独特的艺术风格。

- [ ] 深度缓冲 (Depth Buffer)
  - [ ] 启用 GL_DEPTH_TEST.
  - [ ] 解决 2D 透明物体与 3D 不透明物体的混合排序问题.
- [ ] 风格化 Shader (Stylized Shaders)
  - [ ] Cel-Shading (卡通着色): 实现色阶量化 (Quantization) 和 边缘光 (Rim Light).
  - [ ] Outline (描边): 基于法线扩充或后处理的描边算法.
  - [ ] Dithering (抖动): 模拟复古 PS1 风格的半透明效果.
- [ ] 混合渲染测试: 在 2D 背景图上渲染一个旋转的 3D 卡通角色.

## 🛠️ 阶段四：开发者工具 (Phase 4: Tooling)
**目标：** “工欲善其事，必先利其器”。在做复杂的 C/S 逻辑前，必须有可视化调试工具。

- [ ] **ImGui 集成**
  - [ ] 初始化 ImGui (Docking 分支).
  - [ ] 处理 ImGui 的输入阻断 (当鼠标在 UI 上时，游戏不响应).
- [ ] **核心调试面板**
  - [ ] **Stats Panel**: 实时监控 Draw Calls, FPS, 内存占用.
  - [ ] **Renderer Config**: 实时开关线框模式、调整背景色.
  - [ ] **Log Console**: 在游戏内显示控制台日志 (`spdlog` sink).
- [ ] **场景层级 (Hierarchy)**: 列出当前所有实体，点击可查看详情.

## 📡 阶段五：C/S 架构分离 (Phase 5: Architecture Split)
**目标：** 实现单人游戏的“伪联机”架构。这是 RTS 和大规模计算的基石。

- [ ] **通用数据层**
  - [ ] 定义 `Packet` 结构体 (Type, Data).
  - [ ] 序列化/反序列化 (简单的二进制读写).
- [ ] **通信层 (Loopback)**
  - [ ] 编写线程安全的队列 `ThreadSafeQueue<Packet>`.
  - [ ] 实现 `LoopbackDriver`: 模拟网络延迟和丢包 (用于测试鲁棒性).
- [ ] **服务端 (Server Thread)**
  - [ ] 创建 `GameServer` 类，运行在独立 `std::thread`.
  - [ ] **Tick Loop**: 固定频率 (如 60Hz) 更新逻辑，与渲染帧率解耦.
- [ ] **客户端 (Client)**
  - [ ] **预测与插值**: 客户端先移动，服务器确认；如果不同步则回滚 (Reconciliation).
  - [ ] **测试**: 两个窗口，一个主机一个客机，同步移动方块.

## 🧠 阶段六：游戏性与 AI (Phase 6: Gameplay Foundation)
**目标：** 赋予对象智能。针对 RTS/RPG 的大规模单位设计。

- [ ] **ECS 初步 (或轻量级对象系统)**
  - [ ] 既然是 RTS，建议引入 `entt` 库或手写简单的 Component System.
  - [ ] 分离数据 (Position, Velocity) 与逻辑 (MovementSystem).
- [ ] **AI 状态机 (State Machine)**
  - [ ] **接口设计**: `IState` (Enter, Execute, Exit).
  - [ ] **分层状态机 (HFSM)**: 支持状态嵌套 (如 "Combat" 状态下有 "Attack", "Dodge").
  - [ ] **黑板 (Blackboard)**: AI 共享数据区 (如 "TargetLocation").
- [ ] **寻路基础**: 集成 A* 算法或 Flow Field (流场寻路，适合 RTS 大量单位).

## 🔦 阶段七：2D 光线追踪与特效 (Phase 7: The "Holy Grail")
**目标：** 实现引擎的核心视觉特色。

- [ ] **帧缓冲架构 (FrameBuffers)**
  - [ ] 渲染到纹理 (Render to Texture).
  - [ ] 多重渲染目标 (MRT): 同时输出 颜色、法线、位置信息.
- [ ] **2D SDF 光照 (Ray Tracing)**
  - [ ] **JFA (Jump Flooding Algorithm)**: GPU 实时生成距离场纹理.
  - [ ] **Ray Marching Shader**: 在 Shader 中步进光线，计算遮挡和软阴影.
  - [ ] **GI (全局光照)**: 模拟光线反弹 (Radiance Cascades 原型).
- [ ] **后处理栈 (Post-Processing)**
  - [ ] **Bloom**: 提取高亮区域并模糊.
  - [ ] **Tone Mapping**: HDR 转 LDR.

## 📦 阶段八：打磨与发布 (Phase 8: Polish)
- [ ] **音频系统**: 集成 `miniaudio`，支持 2D 空间音效.
- [ ] **UI 系统**: 绘制 HUD，支持文字渲染 (`stb_truetype`).
- [ ] **项目导出**: 简单的打包脚本，将资源和 exe 打包.

# Phase 2: 架构升级与资源管理

## 目标
从“硬编码渲染”转向“数据驱动渲染”，为 ECS 和高级光照打下基础。

## 1. 资源管理系统 (Asset System)
目前我们直接创建 `Texture2D`。未来我们需要一个中心化的管理器来处理资源的加载、缓存和热重载。

- [ ] **Asset Manager**: 实现 `TextureLibrary` 或 `AssetManager`。
  - [ ] 支持通过路径加载纹理，避免重复加载同一张图。
  - [ ] 使用 `std::string` (路径) 或 `UUID` 作为键。
- [ ] **Shader Library**: 管理 Shader 的编译和热重载。

## 2. 材质系统 (Material System) - 关键一步
为了支持未来的光照，我们需要把“纹理”升级为“材质”。

- [ ] **Material 基础类**:
  - [ ] 属性：`AlbedoMap`, `NormalMap`, `Roughness`, `Metallic`。
  - [ ] 统一接口：`Bind()`。
- [ ] **Renderer 适配**:
  - [ ] 修改 `DrawQuad` 接受 `MaterialInstance` 而非 `Texture2D`。
  - [ ] *挑战*：Batch Rendering 如何处理不同的材质属性？(通常需要根据材质ID进行分批，或者使用 Texture Arrays 存储所有材质贴图)。

## 3. ECS (Entity Component System) 基础
引入 ECS 框架，解耦逻辑与渲染。

- [ ] **引入 EnTT**: 集成 EnTT 库。
- [ ] **Scene 类**: 创建 `Scene` 类作为 ECS 的容器。
- [ ] **基础组件**:
  - [ ] `TagComponent` (名字)
  - [ ] `TransformComponent` (位置、旋转、缩放)
  - [ ] `SpriteRendererComponent` (颜色、纹理/材质)
- [ ] **Scene Hierarchy Panel**: (编辑器UI) 显示实体列表。

## 4. 2D 渲染管线升级 (为光照做准备)
这是通向 2D SDF GI 的前置任务。

- [ ] **Framebuffer 封装**: 完善 `Framebuffer` 类，支持多渲染目标 (MRT)。
- [ ] **G-Buffer Setup**:
  - [ ] 创建一个包含 `ColorAttachment` (RGBA8) 和 `NormalAttachment` (RG16F) 的 Framebuffer。
  - [ ] 修改 `Renderer2D` 将数据写入这个 Framebuffer 而不是默认屏幕。

## 优先级建议
1. **Asset System** (最简单，立刻能用)
2. **ECS 基础** (架构核心，决定了后面代码怎么写)
3. **材质系统 & G-Buffer** (光照核心，可以稍后，等 ECS 跑通了再把 RenderSystem 升级为 Deferred Pipeline)
我还计划融入一些别的引擎的东西, 比如Matirel，Renderer3D，以及Spiter，tilemap等，当然还要有我们最终想得二维光追（2D SDF Global Illumination (GI) 或 Radiance Cascades）。所以我希望你以后的意见不要局限在仿照Hazel直接抄，而是建设性的给一些参考Unreal， Godot的意见，因为我们的目标还是有难度。
---

### 🏆 学习曲线里程碑 (Milestones)

1.  **里程碑 A (The Artist):** 屏幕上显示一张贴图，并且能通过 ImGui 调节它的颜色和透明度。 *(完成阶段 2, 3, 4)*
2.  **里程碑 B (The Architect):** 启动程序后，后台有一个 Server 线程在打印 "Tick"，前台 Client 按下 W 键，Server 收到包并返回新坐标，Client 平滑移动。 *(完成阶段 5)*
3.  **里程碑 C (The Dimension Breaker)**: 在 2D 场景中加载并显示一个 Blender 导出的 3D 模型，并应用卡通渲染 Shader。 (完成阶段 3 & 3.5)
4.  **里程碑 D (The Commander):** 屏幕上有 100 个单位，点击右键，它们自动寻路移动到目标点（RTS 雏形）。 *(完成阶段 6)*
5.  **里程碑 E (The Visionary):** 放置一个点光源，墙壁投射出动态的软阴影，并且光线能照亮拐角（GI）动态软阴影照亮 3D 模型和 2D 精灵。 *(完成阶段 7)*
