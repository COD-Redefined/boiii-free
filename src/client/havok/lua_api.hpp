#pragma once

namespace lua 
{
    struct lua_State;
    struct luaL_Reg 
    {
        const char* name;
        int (*func)(lua_State*);
    };
}