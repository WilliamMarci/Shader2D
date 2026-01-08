#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// #include <glm/gtx/quaternion.hpp>

#include <algorithm>
#include <cmath>
#include <limits>

namespace Engine {

    // --- Vectors ---

    struct Vec2 : public glm::vec2 {
        using glm::vec2::vec2;
        Vec2(const glm::vec2& v) : glm::vec2(v) {}
        static Vec2 Zero() { return Vec2(0.0f); }
        static Vec2 One() { return Vec2(1.0f); }
    };

    struct Vec3 : public glm::vec3 {
        using glm::vec3::vec3;
        Vec3(const glm::vec3& v) : glm::vec3(v) {}
        static Vec3 Zero() { return Vec3(0.0f); }
        static Vec3 One() { return Vec3(1.0f); }
        static Vec3 Up() { return Vec3(0.0f, 1.0f, 0.0f); }
        static Vec3 Right() { return Vec3(1.0f, 0.0f, 0.0f); }
        static Vec3 Forward() { return Vec3(0.0f, 0.0f, -1.0f); }
    };

    struct Vec4 : public glm::vec4 {
        using glm::vec4::vec4;
        Vec4(const glm::vec4& v) : glm::vec4(v) {}
        static Vec4 Zero() { return Vec4(0.0f); }
        static Vec4 One() { return Vec4(1.0f); }
    };

    // --- Matrices ---

    struct Mat4 : public glm::mat4 {
        using glm::mat4::mat4;
        Mat4(const glm::mat4& m) : glm::mat4(m) {}
        static Mat4 Identity() { return Mat4(1.0f); }
        
        static Mat4 Translate(const Vec3& translation) {
            return glm::translate(glm::mat4(1.0f), translation);
        }
        static Mat4 Rotate(float angle, const Vec3& axis) {
            return glm::rotate(glm::mat4(1.0f), angle, axis);
        }
        static Mat4 Scale(const Vec3& scale) {
            return glm::scale(glm::mat4(1.0f), scale);
        }
    };
    
    struct Quat : public glm::quat {
        using glm::quat::quat;
        Quat(const glm::quat& q) : glm::quat(q) {}
    };

    // --- Math Utilities ---
    
    namespace Math {
        // Constants
        constexpr float PI = 3.14159265358979323846f;
        constexpr float TwoPI = PI * 2.0f;
        constexpr float HalfPI = PI * 0.5f;
        constexpr float Epsilon = std::numeric_limits<float>::epsilon();

        // Trigonometry
        inline float Radians(float degrees) { return glm::radians(degrees); }
        inline float Degrees(float radians) { return glm::degrees(radians); }
        
        inline float Sin(float angle) { return std::sin(angle); }
        inline float Cos(float angle) { return std::cos(angle); }
        inline float Tan(float angle) { return std::tan(angle); }
        inline float ASin(float v) { return std::asin(v); }
        inline float ACos(float v) { return std::acos(v); }
        inline float ATan(float v) { return std::atan(v); }
        inline float ATan2(float y, float x) { return std::atan2(y, x); }

        // Basic Math
        inline float Sqrt(float v) { return std::sqrt(v); }
        inline float Pow(float base, float exp) { return std::pow(base, exp); }
        inline float Abs(float v) { return std::abs(v); }
        
        template<typename T>
        inline T Min(T a, T b) { return std::min(a, b); }
        
        template<typename T>
        inline T Max(T a, T b) { return std::max(a, b); }

        template<typename T>
        inline T Clamp(T value, T min, T max) { return std::clamp(value, min, max); }

        template<typename T>
        inline T Lerp(T a, T b, float t) { return a + (b - a) * t; }

        // OpenGL Interop
        template<typename T>
        const float* ValuePtr(const T& v) { return glm::value_ptr(v); }
    }
}