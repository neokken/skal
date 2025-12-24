//
// Created by okker on 20/12/2025.
//

#pragma once

#include "spdlog/spdlog.h"


namespace skal
{

    class Log {
    public:
        static void Initialize();


        template<typename... Args>
        static void Info(spdlog::format_string_t<Args...> fmt, Args&&... args) {
            s_logger->info(fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void Warn(spdlog::format_string_t<Args...> fmt, Args&&... args) {
            s_logger->warn(fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void Error(spdlog::format_string_t<Args...> fmt, Args&&... args) {
            s_logger->error(fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void Critical(spdlog::format_string_t<Args...> fmt, Args&&... args) {
            s_logger->critical(fmt, std::forward<Args>(args)...);
            assert(false && "Critical error occurred");
            std::abort();
        }

    private:
        static std::shared_ptr<spdlog::logger> s_logger;
    };

}