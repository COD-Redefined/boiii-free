#pragma once
#include "game/game.hpp"
#include "loader/component_loader.hpp"

namespace asset_limits
{
    class component final : public generic_component
    {
    public:
        void post_unpack() override;
    };
}
