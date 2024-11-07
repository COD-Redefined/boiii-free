#pragma once
#include "loader/component_interface.hpp"
#include <Windows.h>

namespace t7o
{
    class component final : public generic_component
    {
    public:
        component();
        
        void post_start() override;
        void lua_start() override;
        void pre_destroy() override;
        void start_hooks() override;
        void destroy_hooks() override;
        void post_load() override;
        void post_unpack() override;

        component_priority priority() const override
        {
            return component_priority::min;
        }

    private:
        HMODULE module_{nullptr};
        bool initialized_{false};
    };
}