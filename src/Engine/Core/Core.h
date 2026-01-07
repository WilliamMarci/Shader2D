#pragma once

#include <memory>

// Platform detection using predefined macros
#ifdef _WIN32
    #ifdef _WIN64
        #define ENG_PLATFORM_WINDOWS
    #else
        #error "x86 Builds are not supported!"
    #endif
#elif defined(__APPLE__) || defined(__MACH__)
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR == 1
        #error "IOS simulator is not supported!"
    #elif TARGET_OS_IPHONE == 1
        #error "IOS is not supported!"
    #elif TARGET_OS_MAC == 1
        #define ENG_PLATFORM_MACOS
    #else
        #error "Unknown Apple Platform!"
    #endif
#elif defined(__ANDROID__)
    #define ENG_PLATFORM_ANDROID
    #error "Android is not supported!"
#elif defined(__linux__)
    #define ENG_PLATFORM_LINUX
#else
    #error "Unknown Platform!"
#endif

// Bit shift macro
#define BIT(x) (1 << x)

// Assertions
#ifdef ENG_ENABLE_ASSERTS
    #define ENG_ASSERT(x, ...) { if(!(x)) { ENG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
    #define ENG_CORE_ASSERT(x, ...) { if(!(x)) { ENG_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
    #define ENG_ASSERT(x, ...)
    #define ENG_CORE_ASSERT(x, ...)
#endif

namespace Engine {

    // Smart Pointer Aliases
    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

}
