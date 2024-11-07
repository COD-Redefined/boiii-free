#pragma once
#include "game/game.hpp"
#include "loader/component_loader.hpp"
#include "../component/command.hpp"
#include "console_log.hpp"
#include <utils/hook.hpp>
#include <utils/io.hpp>
#include <vector>

namespace zone_loader 
{
    class component final : public generic_component 
    {
    public:
        void post_unpack() override;
        static bool load_custom_zone(const std::string& name);
        static std::vector<std::string> get_loaded_zones();
        static bool load_zone_file(const std::string& path);
        static void register_zone_commands();
        static std::vector<std::string> scan_available_zones();
        
    private:
        static utils::hook::detour load_zone_hook;
        static std::string find_custom_zone(const std::string& name);
        static game::workshop_data* load_zone_stub(const char* name);
        static std::vector<std::string> loaded_zones_;
    };
}
