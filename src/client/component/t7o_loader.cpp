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
        printf("[T7O] Loader component initialized\n");
        SetUnhandledExceptionFilter(dll_exception_handler);
    }

    void component::post_start()
    {
        if (initialized_) 
        {
            printf("[T7O] Already initialized, skipping\n");
            return;
        }

        printf("[T7O] Attempting to load T7Overcharged.dll...\n");
        if (try_load_dll("plugins\\T7Overcharged.dll"))
        {
            printf("[T7O] Successfully loaded T7Overcharged.dll\n");
            initialized_ = true;
        }
        else
        {
            printf("[T7O] Failed to load T7Overcharged.dll\n");
        }
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
        printf("[T7O] Attempting to load DLL: %s\n", dll_name);
        
        __try 
        {
            HMODULE module = LoadLibraryA(dll_name);
            if (!module)
            {
                printf("[T7O] LoadLibraryA failed with error: %lu\n", GetLastError());
                return false;
            }
            return true;
        }
        __except(dll_exception_handler(GetExceptionInformation()))
        {
            printf("[T7O] Exception while loading DLL\n");
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
