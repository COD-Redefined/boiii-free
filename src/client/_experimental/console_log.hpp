#pragma once
#include "loader/component_interface.hpp"
#include <fstream>
#include <string>

namespace console_log
{
    class component final : public client_component
    {
    public:
        void post_unpack() override;
        ~component();

        static void message_interceptor(const std::string& message);
        static void log_error(const std::string& error);
        static void log_system(const std::string& system);
        static void log_info(const std::string& info);

    private:
        static std::ofstream log_file;
    };
}