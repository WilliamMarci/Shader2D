# Code Style Guide

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
