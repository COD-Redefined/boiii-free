#include <std_include.hpp>
#include "zone_loader.hpp"
#include <utils/hook.hpp>
#include <utils/string.hpp>
#include <utils/io.hpp>

namespace zone_loader
{
    utils::hook::detour component::load_zone_hook;

    std::string component::find_custom_zone(const std::string& name)
    {
        const auto data_folder = game::get_appdata_path() / "data";
        const auto zone_path = (data_folder / "zone" / (name + ".ff")).string();
        const auto usermap_path = (data_folder / "usermaps" / (name + ".ff")).string();

        if (utils::io::file_exists(zone_path))
        {
            return zone_path;
        }
        
        if (utils::io::file_exists(usermap_path))
        {
            return usermap_path;
        }

        return "";
    }

    game::workshop_data* component::load_zone_stub(const char* name)
    {
        const auto custom_path = find_custom_zone(name);
        if (!custom_path.empty())
        {
            printf("Loading custom zone from: %s\n", custom_path.c_str());
            return load_zone_hook.invoke<game::workshop_data*>(custom_path.c_str());
        }
        
        return load_zone_hook.invoke<game::workshop_data*>(name);
    }

    void component::post_unpack()
    {
        try 
        {
            load_zone_hook.create(game::select(0x1420D5700, 0x1404E18B0), load_zone_stub);
            printf("Zone loader hook created successfully\n");
        }
        catch(const std::exception& ex)
        {
            printf("Failed to create zone loader hook: %s\n", ex.what());
        }
    }
}

REGISTER_COMPONENT(zone_loader::component)
