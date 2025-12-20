//
// Created by okker on 20/12/2025.
//

#include "skal/util/log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace skal
{
    std::shared_ptr<spdlog::logger> Log::s_logger;
}

void skal::Log::Initialize()
{
    s_logger = spdlog::stdout_color_mt("SKAL");
    s_logger->set_pattern("[%^%l%$] %v");
    s_logger->set_level(spdlog::level::trace);
}
