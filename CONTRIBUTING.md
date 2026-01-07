# 1.  Code Style Guide

## 1. Directory and File Structure

* Principle: The physical structure (folders) should strictly match the logical structure (namespaces).
* Header files (.h): Must include `#pragma once`.
* Include order:
    1. Corresponding header (e.g., in `GameApp.cpp` the first line is `#include "GameApp.h"`)
    2. Other modules in this project (`#include "Platform/..."`)
    3. Third-party libraries (`#include <glad/glad.h>`, `#include <glm/...>`)
    4. Standard library (`#include <iostream>`, `#include <vector>`)

## 2. Naming Convention

| Type                         | Rule           | Example                          | Notes                            |
| :--------------------------: | :------------- | :------------------------------- | :--------------------------------|
| **Class/Struct**             | PascalCase     | `GameApp`, `BatchRenderer`       | Upper camel case                 |
| **Function/Method**          | PascalCase     | `Update()`, `CompileShader()`    | Upper camel case, verb-first     |
| **Variable (local)**         | camelCase      | `vertexSource`, `windowWidth`    | lower camel case                 |
| **Variable (member)**        | m_PascalCase   | `m_RendererID`, `m_Window`       | `m_` prefix + PascalCase         |
| **Variable (static member)** | s_PascalCase   | `s_Instance`                      | `s_` prefix                      |
| **Constant/Macro**           | SCREAMING_SNAKE| `MAX_SPRITES`, `PI`              | upper case + underscores         |
| **Namespace**                | PascalCase     | `MyEngine`, `Client`             | Avoid abbreviations              |
| Variable (Public Struct member)| PascalCase   | `Position`, `Color`              | Only for POD structs             |

Special case: For uppercase acronyms (like `VAO` or `JSON`), keep them uppercase in class names (`JSONParser`), but treat them as a word in variable names (`jsonParser`).

## 3. Modern C++ Usage

We enforce the **C++17** standard.

* Memory management:
    * It is forbidden to use raw `new`/`delete` in application logic.
    * Smart pointers must be used:
        * Use `std::unique_ptr` for exclusive ownership (e.g., `m_Window`, `m_Shader`).
        * Use `std::shared_ptr` for shared ownership (e.g., future `Texture`, `Material`).
    * Exceptions: low-level wrappers (e.g., inside `Window` calling GLFW) or observer patterns (passing `Window*` to Input) may use raw pointers but must not own/destroy them.
* Keywords:
    * Overridden virtual functions must use `override`.
    * Functions that do not modify member variables must be `const` (e.g., `IsKeyPressed`, `GetWidth`).
    * Single-argument constructors should be `explicit` to prevent implicit conversions.

## 4. Class Design

* RAII (Resource Acquisition Is Initialization):
    * Constructors should acquire resources (e.g., `glGenBuffers`).
    * Destructors should release resources (e.g., `glDeleteBuffers`).
    * Resource leaks are unacceptable.
* Pimpl (optional): For complex low-level implementations (e.g., heavy OpenGL headers), prefer implementing details in `.cpp` to keep `.h` clean.

## 5. Rendering and Math

* Math library: Use **GLM** consistently.
    * Vectors: `glm::vec2`, `glm::vec3`, `glm::vec4`
    * Matrices: `glm::mat4`
* OpenGL calls:
    * Do not call low-level APIs like `glDrawArrays` directly from high-level code such as `GameApp`.
    * Always call through wrapper classes like `Renderer` or `Shader`.

## 6. Comments and Documentation

* Header comments: Explain what the class does.
* Method comments: If logic is complex, explain why it is done this way, not what is done (the code shows that).
* TODO: Mark with `// TODO: description`.

## 7. Type Casting

* C-style casts `(type)value` are forbidden.
* Use C++ casts according to intent:
    * `static_cast`: for safe conversions (e.g., float to int, derived to base).
    * `reinterpret_cast`: for low-level bit operations (e.g., long to void*, OpenGL offset handling).
    * `const_cast`: avoid; only for interfacing with legacy libraries.
    * `dynamic_cast`: avoid when possible (performance overhead); use only in tooling layers.

## 8. Data Types and Primitives

To ensure cross-platform compatibility and correct memory layout for GPU data, strict type conventions are enforced.

* **Integers**:
    * Use `<cstdint>` fixed-width types for class members, serialization, and GPU data.
        * `uint8_t`: Bytes, colors (0-255).
        * `uint16_t`: Large indices (if > 256 and < 65536).
        * `uint32_t`: General IDs, handles, counts, sizes in headers.
        * `int32_t`: Signed integers requiring fixed width.
    * Use `int` only for trivial local loops or return codes where size is irrelevant.
    * Use `size_t` for memory sizes, array indexing, and loop counters iterating over containers.

* **Floating Point**:
    * Use `float` for all rendering math, physics, and transforms.
    * Avoid `double` unless high-precision scientific calculation is required (rare in rendering).

* **OpenGL Types**:
    * In **Platform/OpenGL** files: Use `GLenum`, `GLuint`, `GLint` to match GLAD signatures.
    * In **Engine/Renderer** (Abstraction) files: Use `uint32_t` instead of `GLuint` to maintain API independence.

* **Strings**:
    * Use `std::string` for general text.
    * Use `const char*` only for static string literals or interfacing with C-APIs (like ImGui or OpenGL).

# 2. Design Philosophy & Architecture

## 2.1 The "Brain" vs. "Limbs" Separation
The engine is strictly divided into two layers to ensure cross-platform capability without polluting the core logic.

*   **Engine (The Brain)**: Located in `src/Engine`.
    *   Defines **Interfaces** (What to do).
    *   Handles high-level logic (Scene management, ECS, Math, Physics simulation).
    *   **Rule**: Never include platform-specific headers (e.g., `<GLFW/glfw3.h>`, `<glad/glad.h>`, `<X11/...>`) in Engine header files.
    *   **Rule**: Use `void*` or incomplete types if a handle must be stored.

*   **Platform (The Limbs)**: Located in `src/Platform`.
    *   Implements **Interfaces** (How to do it).
    *   Interacts directly with hardware drivers or OS APIs.
    *   **Rule**: Organized by **Functionality/Technology**, not by Operating System.
        *   ✅ `Platform/OpenGL` (Graphics implementation)
        *   ✅ `Platform/Desktop` (Windowing & Input via GLFW)
        *   ❌ `Platform/Linux` (Avoid OS-based folder names)

## 2.2 Zero Ping-Pong Dependency
We strictly forbid the "Ping-Pong" calling pattern (`Engine` calls `Platform` wrapper, which calls another `Engine` utility, which calls `Platform`...).

*   **Implementation Ownership**: The Platform layer should contain the **full implementation** of a feature.
    *   *Bad*: `Engine::Input` calls `Platform::WindowsInput`, which calls `Engine::KeyConverter`, which calls `GLFW`.
    *   *Good*: `Platform::DesktopInput` directly implements `Engine::Input` interface using GLFW calls.
*   **Factory Pattern**: The Engine only knows about the interface (e.g., `Window`). The instantiation happens in the `.cpp` file using a Factory method (e.g., `Window::Create()`), controlled by build macros.

## 2.3 Functional Organization over OS Separation
Since we primarily develop on Linux but aim for portability:
*   Do not create separate files for `LinuxWindow.cpp` and `WindowsWindow.cpp` if they share 90% of the code (e.g., via GLFW).
*   Use generic names like `DesktopWindow.cpp`.
*   Platform-specific nuances (if any) should be handled via `#ifdef` macros *inside* the generic implementation file, or by specific build system exclusions, rather than duplicating files.

## 2.4 Render Hardware Interface (RHI)
*   **Renderer** (Engine) decides **what** to draw (sorting, culling, batching).
*   **OpenGL/Vulkan** (Platform) decides **how** to submit commands to the GPU.
*   The `Renderer` should not issue direct `glDraw...` calls. It should submit `RenderCommands` to the `RendererAPI`.

## 2.5 Strict Initialization Order
1.  **Log System**: Must be initialized first.
2.  **Window/Context**: Required for graphics context.
3.  **Renderer Core**: Capabilities check, shader compilation.
4.  **Application/Layers**: Game logic.
