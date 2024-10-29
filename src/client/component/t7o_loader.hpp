#pragma once
#include "loader/component_interface.hpp"
#include <Windows.h>

namespace t7o
{
    class component final : 
        public component_interface,
        public generic_component
    {
    public:
        component();
        
        // component_interface overrides
        void post_start() override;
        void lua_start() override;
        void pre_destroy() override;
        void start_hooks() override;
        void destroy_hooks() override;

        // generic_component overrides
        void post_load() override;
        void post_unpack() override;

        component_priority priority() const override
        {
            return component_priority::min;
        }

    private:
        bool try_load_dll(const char* dll_name);
        static LONG WINAPI dll_exception_handler(LPEXCEPTION_POINTERS ex_info);
        
        HMODULE module_{nullptr};
        bool initialized_{false};
    };
}