#include "glacie/api/memory/Hook.h"

#include "mc/network/ServerNetworkHandler.h"
#include "mc/options/FeatureToggles.h"
#include "mc/server/PropertiesSettings.h"

// 强制开启方块NbtHash RuntimeID
LL_AUTO_TYPED_INSTANCE_HOOK(
    BlockNetworkIdsAreHashes,
    HookPriority::Normal,
    PropertiesSettings,
    &PropertiesSettings::blockNetworkIdsAreHashes,
    bool
) {
    return true;
}