//
// Created by okker on 20/12/2025.
//

#include "skal/util/log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace skal
{
    std::shared_ptr<spdlog::logger> Log::s_logger = nullptr;
    Log::LogCallback Log::s_callback = nullptr;
}

void skal::Log::Initialize()
{
    if (s_logger)
    {
        return;
    }
    s_logger = spdlog::stdout_color_mt("SKAL");
    s_logger->set_pattern("[%^%l%$] %v");
    s_logger->set_level(spdlog::level::trace);
}

void skal::Log::SetCallback(const LogCallback& callback)
{
    s_callback = callback;
}
