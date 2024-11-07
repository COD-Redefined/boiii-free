#include <std_include.hpp>
#include "zone_loader.hpp"
#include <utils/hook.hpp>
#include <utils/string.hpp>
#include <utils/io.hpp>
#include "../component/command.hpp"
#include "game/game.hpp"

namespace zone_loader
{
    namespace
    {
        utils::hook::detour db_load_xfile_hook;

        void db_load_xfile_stub(const char* name, int type)
        {
            // Remove en_ prefix if present
            std::string clean_name = name;
            if (clean_name.substr(0, 3) == "en_")
            {
                clean_name = clean_name.substr(3);
            }
            
            db_load_xfile_hook.invoke<void>(clean_name.c_str(), type);
        }
    }

    bool component::load_zone_file(const std::string& name)
    {
        game::XZoneInfo info{};
        info.name = name.c_str();
        info.allocFlags = 0x1;
        info.freeFlags = 0x0;
        
        utils::hook::invoke<void>(game::select(0x1420D5700, 0x1404E18B0), 1, &info, false);
        return true;
    }

    void component::post_unpack()
    {
        // Hook DB_LoadXFile which is called before zone loading
        db_load_xfile_hook.create(0x1420D5200_g, db_load_xfile_stub);

        command::add("loadZone", [](const command::params& params)
        {
            if (params.size() < 2)
            {
                game::show_error("Usage: loadZone <zone_name>");
                return;
            }

            const auto zone_name = params.get(1);
            if (load_zone_file(zone_name))
            {
                game::show_error("Loaded zone: %s", zone_name);
            }
        });
    }
}

REGISTER_COMPONENT(zone_loader::component)
