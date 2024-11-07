#pragma once
#include "std_include.hpp"
#include "loader/component_loader.hpp"
#include "console_log.hpp"
#include "game/game.hpp"
#include <utils/io.hpp>

namespace zone_monitor {
    class component final : public generic_component {
    public:
        void post_unpack() override;
        static std::vector<std::string> scan_zone_directories();
        
    private:
        static bool is_valid_zone_file(const std::string& path);
    };
}
