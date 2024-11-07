#pragma once
#include "game/game.hpp"
#include "loader/component_loader.hpp"
#include "../component/command.hpp"
#include <utils/hook.hpp>
#include <utils/io.hpp>

namespace zone_loader 
{
    class component final : public generic_component 
    {
    public:
        void post_unpack() override;
        
    private:
        static utils::hook::detour load_zone_hook;
        static game::workshop_data* load_zone_stub(const char* name);
        static std::string find_custom_zone(const std::string& name);
    };
}
