#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "game/game.hpp"
#include <utils/hook.hpp>
#include "asset_limits.hpp"
#include "../_experimental/asset_logger.hpp"

namespace asset_limits
{
    namespace
    {
        void extend_asset_pool()
        {
            const auto results = "48 8B 05 ? ? ? ? 48 8D 1D ? ? ? ? 48 89 78 ? 48 89 38"_sig;
            
            asset_system::asset_logger::log_asset_info();
        }
    }

    void component::post_unpack()
    {
        if (!game::is_client()) return;
        extend_asset_pool();
    }
}

REGISTER_COMPONENT(asset_limits::component)
