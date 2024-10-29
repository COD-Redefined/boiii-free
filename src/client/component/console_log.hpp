#pragma once
#include "loader/component_interface.hpp"
#include <fstream>

namespace console_log
{
    class component final : public client_component
    {
    public:
        void post_unpack() override;
        ~component();

    private:
        static void message_interceptor(const std::string& message);
        static std::ofstream log_file;
    };
}