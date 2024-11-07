#include <std_include.hpp>
#include "dll_loader.hpp"
#include "game/game.hpp"
#include <filesystem>
#include "../component/command.hpp"
#include "../_experimental/console_log.hpp"
#include <utils/nt.hpp>
#include "../component/game_event.hpp"

namespace t7o
{
    namespace
    {
        utils::nt::library codale_dll{};

        bool load_plugin(const std::string& name)
        {
            try
            {
                const auto plugins_path = std::filesystem::path(game::get_appdata_path()) / "plugins";
                codale_dll = utils::nt::library::load(plugins_path / name);
                
                const auto init_func = codale_dll.get_proc<bool(*)(void*)>("init");
                if (!init_func || !init_func(nullptr))
                {
                    console_log::component::log_info("Plugin initialization failed: " + name);
                    codale_dll.free();
                    return false;
                }

                return true;
            }
            catch(...)
            {
                console_log::component::log_info("Failed to load plugin: " + name);
                return false;
            }
        }

        void load_codale_plugin()
        {
            if (load_plugin("codale.dll"))
            {
                console_log::component::log_info("Auto-loaded codale.dll");
            }
        }
    }

    component::component()
    {
        const auto plugins_path = std::filesystem::path(game::get_appdata_path()) / "plugins";
        std::filesystem::create_directories(plugins_path);
    }

    void component::post_unpack()
    {
        // Wait for game initialization to complete
        game_event::on_g_init_game(load_codale_plugin);

        command::add("loadPlugin", [](const command::params& params)
        {
            if (params.size() < 2)
            {
                console_log::component::log_info("Usage: loadPlugin <name>");
                return;
            }

            const auto plugin_name = params.get(1);
            if (load_plugin(plugin_name))
            {
                console_log::component::log_info("Plugin loaded: " + std::string(plugin_name));
            }
        });
    }

    void component::post_load()
    {
        // Implementation required by interface but not used
    }

    void component::pre_destroy()
    {
        codale_dll.free();
    }
}

REGISTER_COMPONENT(t7o::component)
