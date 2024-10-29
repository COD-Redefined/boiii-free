#include <std_include.hpp>
#include "t7o_loader.hpp"
#include "game/game.hpp"
#include "loader/component_loader.hpp"
#include <utils/hook.hpp>
#include <utils/string.hpp>

namespace t7o
{
    component::component()
    {
        SetUnhandledExceptionFilter(dll_exception_handler);
    }

    void component::post_start()
    {
        if (initialized_) return;
        try_load_dll("plugins\\T7Overcharged.dll");
    }

    void component::post_load() {}
    void component::post_unpack() {}
    void component::lua_start() {}
    void component::start_hooks() {}
    void component::destroy_hooks() {}

    LONG WINAPI component::dll_exception_handler(LPEXCEPTION_POINTERS ex_info)
    {
        if (ex_info->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
        {
            printf("[T7O] Access violation caught at address: %p\n", 
                ex_info->ExceptionRecord->ExceptionAddress);
            return EXCEPTION_EXECUTE_HANDLER;
        }
        return EXCEPTION_CONTINUE_SEARCH;
    }

    bool component::try_load_dll(const char* dll_name)
    {
        __try 
        {
            printf("[T7O] Attempting to load %s\n", dll_name);
            module_ = LoadLibraryA(dll_name);
            
            if (!module_)
            {
                printf("[T7O] Failed to load %s (Error: %lu)\n", dll_name, GetLastError());
                return false;
            }

            using init_func_t = int(*)(void*);
            auto init_func = reinterpret_cast<init_func_t>(GetProcAddress(module_, "init"));
            if (!init_func)
            {
                printf("[T7O] Failed to find init function in %s\n", dll_name);
                FreeLibrary(module_);
                module_ = nullptr;
                return false;
            }

            if (!init_func(nullptr))
            {
                printf("[T7O] Init function failed for %s\n", dll_name);
                FreeLibrary(module_);
                module_ = nullptr;
                return false;
            }

            printf("[T7O] Successfully loaded and initialized %s\n", dll_name);
            initialized_ = true;
            return true;
        }
        __except(dll_exception_handler(GetExceptionInformation()))
        {
            printf("[T7O] Exception while loading %s\n", dll_name);
            return false;
        }
    }

    void component::pre_destroy()
    {
        if (module_)
        {
            FreeLibrary(module_);
            module_ = nullptr;
            initialized_ = false;
        }
    }
}

REGISTER_COMPONENT(t7o::component);
