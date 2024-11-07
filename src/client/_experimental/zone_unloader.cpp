#include <std_include.hpp>
#include "zone_unloader.hpp"
#include <utils/hook.hpp>
#include <utils/string.hpp>
#include <utils/io.hpp>
#include "../component/command.hpp"

namespace zone_unloader
{
    const std::vector<std::string> component::campaign_fastfiles = {
        "cp_common",
        "cp_patch",
        "cp_frontend",
        "cp_prologue",
        "cp_mi_eth_prologue",
        "cp_mi_sing_biodomes",
        "cp_mi_sing_biodomes2",
        "cp_mi_sing_blackstation",
        "cp_mi_sing_vengeance"
    };

    const std::vector<std::string> component::zombie_fastfiles = {
        "zm_common",
        "zm_patch",
        "zm_frontend",
        "zm_factory",
        "zm_zod",
        "zm_castle",
        "zm_island",
        "zm_stalingrad",
        "zm_genesis",
        "zm_prototype"
    };

    bool component::unload_fastfile(const std::string& name)
    {
        game::XZoneInfo info{};
        info.name = name.c_str();
        info.allocFlags = 0x1;
        info.freeFlags = 0x1;
        
        utils::hook::invoke<void>(game::select(0x1420D5700, 0x1404E18B0), 1, &info, true);
        return true;
    }

    bool component::unload_mode_fastfiles(const game::eModes mode)
    {
        const auto& files_to_unload = (mode == game::MODE_CAMPAIGN) ? campaign_fastfiles : zombie_fastfiles;
        
        bool success = true;
        for (const auto& file : files_to_unload)
        {
            if (!unload_fastfile(file))
            {
                success = false;
                break;
            }
        }

        return success;
    }

    void component::post_unpack()
    {
        command::add("unloadCampaign", []()
        {
            if (!game::Com_IsRunningUILevel() && game::Com_SessionMode_GetMode() != game::MODE_CAMPAIGN)
            {
                game::show_error("Must be in Campaign or Main Menu to unload campaign files");
                return;
            }
            unload_all_campaign_files();
        });

        command::add("unloadZombies", []()
        {
            if (!game::Com_IsRunningUILevel() && game::Com_SessionMode_GetMode() != game::MODE_ZOMBIES)
            {
                game::show_error("Must be in Zombies or Main Menu to unload zombie files");
                return;
            }
            unload_all_zombie_files();
        });
    }

    bool component::unload_all_campaign_files()
    {
        return unload_mode_fastfiles(game::MODE_CAMPAIGN);
    }

    bool component::unload_all_zombie_files()
    {
        return unload_mode_fastfiles(game::MODE_ZOMBIES);
    }
}

REGISTER_COMPONENT(zone_unloader::component)