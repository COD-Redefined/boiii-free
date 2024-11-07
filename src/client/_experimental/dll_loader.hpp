#pragma once
#include "loader/component_loader.hpp"
#include <Windows.h>

namespace t7o
{
    class component final : public generic_component
    {
    public:
        component();
        void post_unpack() override;
        void post_load() override;
        void pre_destroy() override;

        component_priority priority() const override
        {
            return component_priority::min;
        }
    };
}