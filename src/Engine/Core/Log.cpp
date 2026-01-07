#include "Log.h"
#include "pch.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Engine {
std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

void Log::Init() {
    // spdlog::set_pattern("%^[%T] %n: %v%$");
    spdlog::set_pattern("[%M:%S.%e %n]%^[%l]%$: %v ", spdlog::pattern_time_type::local);
    s_CoreLogger = spdlog::stdout_color_mt("ENGINE");
    s_CoreLogger->set_level(spdlog::level::trace);

    s_ClientLogger = spdlog::stdout_color_mt("APP");
    s_ClientLogger->set_level(spdlog::level::trace);
}
} 