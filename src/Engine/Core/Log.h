#pragma once

#include <memory>
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

namespace Engine {
class Log {
  public:
    static void Init();
    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
    inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

  private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
};
} // namespace Engine

// Core log macros
#define ENG_CORE_TRACE(...) ::Engine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define ENG_CORE_INFO(...) ::Engine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define ENG_CORE_WARN(...) ::Engine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define ENG_CORE_ERROR(...) ::Engine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define ENG_CORE_FATAL(...) ::Engine::Log::GetCoreLogger()->critical(__VA_ARGS__)
// Client log macros
#define ENG_TRACE(...) ::Engine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define ENG_INFO(...) ::Engine::Log::GetClientLogger()->info(__VA_ARGS__)
#define ENG_WARN(...) ::Engine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ENG_ERROR(...) ::Engine::Log::GetClientLogger()->error(__VA_ARGS__)
#define ENG_FATAL(...) ::Engine::Log::GetClientLogger()->critical(__VA_ARGS__)

#ifdef ENG_ENABLE_ASSERTS
    #define ENG_ASSERT(x, ...) { if(!(x)) { HZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
    #define ENG_CORE_ASSERT(x, ...) { if(!(x)) { HZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
    #define ENG_ASSERT(x, ...)
    #define ENG_CORE_ASSERT(x, ...)
#endif