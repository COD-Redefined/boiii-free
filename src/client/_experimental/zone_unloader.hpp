#pragma once
#include "game/game.hpp"
#include "loader/component_loader.hpp"
#include "../component/command.hpp"
#include <utils/hook.hpp>
#include <utils/io.hpp>

namespace zone_unloader 
{
    class component final : public generic_component 
    {
    public:
        void post_unpack() override;
        static bool unload_mode_fastfiles(game::eModes mode);
        static bool unload_all_campaign_files();
        static bool unload_all_zombie_files();

    private:
        static const std::vector<std::string> campaign_fastfiles;
        static const std::vector<std::string> zombie_fastfiles;
        static bool unload_fastfile(const std::string& name);
    };
}