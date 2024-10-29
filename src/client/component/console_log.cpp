#include <std_include.hpp>
#include "console_log.hpp"
#include "console.hpp"
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
        const auto tm = std::localtime(&now);
        
        // Write timestamped message to file
        if (tm)
        {
            char timestamp[32];
            std::strftime(timestamp, sizeof(timestamp), "[%H:%M:%S] ", tm);
            log_file << timestamp << message;
            log_file.flush();
        }
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
}

REGISTER_COMPONENT(console_log::component)