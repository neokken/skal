//
// Created by okker on 20/12/2025.
//

#pragma once

#include "spdlog/spdlog.h"


namespace skal
{
    enum class LogLevel { Info, Warning, Error, Critical };

    class Log {
    public:
        using LogCallback = std::function<void(LogLevel, const std::string&)>;

        static void Initialize();
        static void SetCallback(const LogCallback &callback);

        template<typename... Args>
        static void Info(spdlog::format_string_t<Args...> fmt, Args&&... args) {
            s_logger->info(fmt, std::forward<Args>(args)...);
            if (s_callback)
            {
                const std::string msg = fmt::format(fmt, std::forward<Args>(args)...);
                s_callback(LogLevel::Info, msg);
            }
        }

        template<typename... Args>
        static void Warn(spdlog::format_string_t<Args...> fmt, Args&&... args) {
            s_logger->warn(fmt, std::forward<Args>(args)...);
            if (s_callback)
            {
                const std::string msg = fmt::format(fmt, std::forward<Args>(args)...);
                s_callback(LogLevel::Warning, msg);
            }
        }

        template<typename... Args>
        static void Error(spdlog::format_string_t<Args...> fmt, Args&&... args) {
            s_logger->error(fmt, std::forward<Args>(args)...);
            if (s_callback)
            {
                const std::string msg = fmt::format(fmt, std::forward<Args>(args)...);
                s_callback(LogLevel::Error, msg);
            }
        }

        template<typename... Args>
        static void Critical(spdlog::format_string_t<Args...> fmt, Args&&... args) {
            s_logger->critical(fmt, std::forward<Args>(args)...);
            if (s_callback)
            {
                const std::string msg = fmt::format(fmt, std::forward<Args>(args)...);
                s_callback(LogLevel::Critical, msg);
            }
            assert(false && "Critical error occurred");
            std::abort();
        }

    private:
        static std::shared_ptr<spdlog::logger> s_logger;
        static LogCallback s_callback;
    };

}