#include "glacie/core/internal/Logger.h"

std::unordered_map<uint64, int> PlayerGuidMap;
std::unordered_set<int>         SupportProtocolList = {594, 618, 622, 630};

LL_AUTO_TYPED_INSTANCE_HOOK(
    RequestNetworkSettingsPacketHandle,
    HookPriority::Normal,
    ServerNetworkHandler,
    "?handle@ServerNetworkHandler@@UEAAXAEBVNetworkIdentifier@@AEBVRequestNetworkSettingsPacket@@@Z",
    void,
    NetworkIdentifier*            a1,
    RequestNetworkSettingsPacket* a2
) {
    if (SupportProtocolList.count(a2->mClientNetworkVersion)) {
        Info("Client Connecting, Guid: {}, Protocol Version: {}", a1->mGuid.g, a2->mClientNetworkVersion);
        PlayerGuidMap[a1->mGuid.g] = a2->mClientNetworkVersion;
        a2->mClientNetworkVersion  = SharedConstants::NetworkProtocolVersion;
    } else {
        Info("Client Disconnected, Guid: {}, Unsupported Protocol Version: {}", a1->mGuid.g, a2->mClientNetworkVersion);
    }
    return origin(a1, a2);
}

LL_AUTO_TYPED_INSTANCE_HOOK(
    UpdateBlockForAllPlayers,
    HookPriority::Normal,
    LoopbackPacketSender,
    "?sendToClients@LoopbackPacketSender@@UEAAXAEBV?$vector@UNetworkIdentifierWithSubId@@V?$allocator@"
    "UNetworkIdentifierWithSubId@@@std@@@std@@AEBVPacket@@@Z",
    void,
    std::vector<NetworkIdentifierWithSubId>& a1,
    Packet*                                  a2
) {
    for (auto pl : a1) { this->sendToClient(pl.id, *a2, pl.subClientId); }
}

LL_AUTO_TYPED_INSTANCE_HOOK(
    ServerNetworkHandlerAllowIncomingPacketId,
    HookPriority::Normal,
    ServerNetworkHandler,
    "?allowIncomingPacketId@ServerNetworkHandler@@UEAA_NAEBVNetworkIdentifier@@W4MinecraftPacketIds@@@Z",
    bool,
    class NetworkIdentifier const& a1,
    ::MinecraftPacketIds           packetId
) {
    ServerNetworkHandler* self = reinterpret_cast<ServerNetworkHandler*>(reinterpret_cast<char*>(this) - 16);

    if ((int)packetId >= 308) {
        std::string message = "disconnectionScreen.unknownPacket";
        self->disconnectClient(a1, SubClientId::PrimaryClient, Connection::DisconnectFailReason(48), message, false);
        return false;
    }
    return true;
}

LL_AUTO_TYPED_INSTANCE_HOOK(
    PacketSend,
    HookPriority::Normal,
    NetworkSystem,
    "?send@NetworkSystem@@QEAAXAEBVNetworkIdentifier@@AEBVPacket@@W4SubClientId@@@Z",
    void,
    NetworkIdentifier* a1,
    Packet*            a2,
    int                a3
) {
    a2->mProtocolVersion = PlayerGuidMap[a1->mGuid.g];
    return origin(a1, a2, a3);
}

uint64 GlobalGuid;

LL_AUTO_TYPED_INSTANCE_HOOK(
    NetworkSystemSortAndPacketizeEvents,
    HookPriority::Normal,
    NetworkSystem,
    "?_sortAndPacketizeEvents@NetworkSystem@@AEAA_NAEAVNetworkConnection@@V?$time_point@Usteady_clock@chrono@std@@V?$"
    "duration@_JU?$ratio@$00$0DLJKMKAA@@std@@@23@@chrono@std@@@Z",
    bool,
    NetworkConnection*                    a1,
    std::chrono::steady_clock::time_point a2
) {
    GlobalGuid = 0;
    auto id    = (NetworkIdentifier*)a1;
    bool result;
    if (id) {
        GlobalGuid = id->mGuid.g;
        result     = origin(a1, a2);
        GlobalGuid = 0;
    } else {
        result = origin(a1, a2);
    };
    return result;
}

LL_AUTO_TYPED_INSTANCE_HOOK(
    PacketRead,
    HookPriority::Normal,
    Packet,
    "?read@Packet@@UEAA?AV?$Result@XVerror_code@std@@@Bedrock@@AEAVReadOnlyBinaryStream@@@Z",
    Bedrock::Result<void>,
    ReadOnlyBinaryStream& a1
) {
    if (GlobalGuid != 0) {
        if (PlayerGuidMap.count(GlobalGuid)) { this->mProtocolVersion = PlayerGuidMap[GlobalGuid]; }
    }
    return origin(a1);
}

LL_AUTO_TYPED_INSTANCE_HOOK(
    SendToClientsHook,
    HookPriority::Normal,
    LoopbackPacketSender,
    "?sendToClients@LoopbackPacketSender@@UEAAXAEBV?$vector@UNetworkIdentifierWithSubId@@V?$allocator@"
    "UNetworkIdentifierWithSubId@@@std@@@std@@AEBVPacket@@@Z",
    void,
    std::vector<NetworkIdentifierWithSubId>& a1,
    Packet*                                  a2
) {
    for (auto pl : a1) { this->sendToClient(pl.id, *a2, pl.subClientId); }
}