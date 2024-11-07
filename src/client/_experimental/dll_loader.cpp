#include <std_include.hpp>
#include "dll_loader.hpp"
#include "game/game.hpp"
#include <filesystem>

namespace t7o
{
    component::component() : initialized_(false), module_(nullptr)
    {
        printf("T7O Component created\n");
    }

    void component::post_start()
    {
        if (initialized_) return;
        
        std::filesystem::path plugins_path = std::filesystem::path(game::get_appdata_path()) / "plugins";
        std::filesystem::path dll_path = plugins_path / "CoDALE.dll";
        
        printf("T7O Loading DLL from: %s\n", dll_path.string().c_str());
        
        std::filesystem::create_directories(plugins_path);
        
        module_ = LoadLibraryA(dll_path.string().c_str());
        if (!module_)
        {
            printf("T7O Failed to load DLL (Error: %lu)\n", GetLastError());
            return;
        }

        using init_func_t = int(*)(void*);
        auto init_func = reinterpret_cast<init_func_t>(GetProcAddress(module_, "init"));
        if (!init_func)
        {
            printf("T7O Failed to find init function\n");
            FreeLibrary(module_);
            module_ = nullptr;
            return;
        }

        if (!game::Com_IsRunningUILevel())
        {
            printf("T7O Failed: UI not initialized\n");
            FreeLibrary(module_);
            module_ = nullptr;
            return;
        }

        printf("T7O Successfully loaded\n");
        initialized_ = true;
    }

    void component::lua_start() {}
    void component::pre_destroy() 
    {
        if (module_)
        {
            FreeLibrary(module_);
            module_ = nullptr;
        }
    }
    void component::start_hooks() {}
    void component::destroy_hooks() {}
    void component::post_load() {}
    void component::post_unpack() {}
}

//REGISTER_COMPONENT(t7o::component)
