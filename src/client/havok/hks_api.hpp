#pragma once
#include "game/game.hpp"
#include "game/structs.hpp"
#include "lua_api.hpp"
#define WEAK __declspec(selectany)
#define OFFSET(address) (uintptr_t)((address - 0x140000000) + (uintptr_t)GetModuleHandle(NULL))

namespace game
{
    WEAK symbol<lua::lua_State*> UI_luaVM{OFFSET(0x14C19C748)};
}

namespace hks
{
    WEAK game::symbol<void (lua::lua_State* s, const char* libname, const lua::luaL_Reg l[], int nup, int isHksFunc)> hksI_openlib { OFFSET(0x141D49440) };
    
    inline lua::lua_State* get_current_state()
    {
        return *game::UI_luaVM;
    }
}