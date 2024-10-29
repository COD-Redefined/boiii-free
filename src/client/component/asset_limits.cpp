#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "game/game.hpp"
#include <utils/hook.hpp>
#include "asset_limits.hpp"

namespace asset_limits
{
    namespace
    {
        void extend_asset_pool()
        {
            // Signature pattern to find the asset pool
            const auto results = "48 8B 05 ? ? ? ? 48 8D 1D ? ? ? ? 48 89 78 ? 48 89 38"_sig;
            
        }
    }

    void component::post_unpack()
    {
        if (!game::is_client()) return;
        extend_asset_pool();
    }
}

REGISTER_COMPONENT(asset_limits::component)
