#define _CRT_SECURE_NO_WARNINGS

#include <std_include.hpp>
#include "console_log.hpp"
#include "component/console.hpp"
#include "loader/component_loader.hpp"
#include <utils/hook.hpp>
#include <utils/string.hpp>
#include <utils/io.hpp>
#include <chrono>
#include <ctime>

namespace console_log
{
    std::ofstream component::log_file;

    void component::message_interceptor(const std::string& message)
    {
        if (!log_file.is_open())
        {
            log_file.open("codredefined_log.txt", std::ios::out | std::ios::app);
        }

        // Get current time
        const auto now = std::time(nullptr);
        std::tm tm_buf;
        localtime_s(&tm_buf, &now);
        
        char timestamp[32];
        std::strftime(timestamp, sizeof(timestamp), "[%H:%M:%S] ", &tm_buf);
        log_file << timestamp << message << std::endl;
        log_file.flush();
    }

    void component::post_unpack()
    {
        console::set_interceptor(message_interceptor);
    }

    component::~component()
    {
        if (log_file.is_open())
        {
            log_file.close();
        }
    }

    void component::log_error(const std::string& error) {
        message_interceptor("[ERROR] " + error);
    }

    void component::log_system(const std::string& system) {
        message_interceptor("[SYSTEM] " + system);
    }

    void component::log_info(const std::string& info) {
        message_interceptor("[INFO] " + info);
    }
}

REGISTER_COMPONENT(console_log::component)
