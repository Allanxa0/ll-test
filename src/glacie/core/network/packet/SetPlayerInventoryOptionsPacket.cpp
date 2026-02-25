
#include <unordered_map>
ServerNetworkHandler* TempHandler;
NetworkIdentifier*    TempNetworkId;
struct InventorySettings {
    int  leftTab;
    int  rightTab;
    bool filtering;
    int  layout;
    int  craftingLayout;
} TempSetting;
std::unordered_map<long long, InventorySettings> PlayerInventorySettings;
class SetPlayerInventoryOptionsPacket : public Packet {
public:
    int  leftTab;
    int  rightTab;
    bool filtering;
    int  layout;
    int  craftingLayout;

    SetPlayerInventoryOptionsPacket() {};
    SetPlayerInventoryOptionsPacket(int leftTab, int rightTab, bool filtering, int layout, int craftingLayout)
    : leftTab(leftTab),
      rightTab(rightTab),
      filtering(filtering),
      layout(layout),
      craftingLayout(craftingLayout) {};

    void write(BinaryStream& bs) const {
        bs.writeVarInt(leftTab);
        bs.writeVarInt(rightTab);
        bs.writeBool(filtering);
        bs.writeVarInt(layout);
        bs.writeVarInt(craftingLayout);
    }

    virtual class Bedrock::Result<void> _read(class ReadOnlyBinaryStream& a1) {
        leftTab                    = a1.getVarInt().value();
        rightTab                   = a1.getVarInt().value();
        filtering                  = a1.getBool().value();
        layout                     = a1.getVarInt().value();
        craftingLayout             = a1.getVarInt().value();
        TempSetting.leftTab        = leftTab;
        TempSetting.rightTab       = rightTab;
        TempSetting.filtering      = filtering;
        TempSetting.layout         = layout;
        TempSetting.craftingLayout = craftingLayout;
        auto pl                    = TempHandler->getServerPlayer(*TempNetworkId, SubClientId::PrimaryClient);
        auto id                    = (long long)pl->getOrCreateUniqueID();
        if (PlayerInventorySettings.count(id)) {
            PlayerInventorySettings[id] = TempSetting;
        } else {
            PlayerInventorySettings.insert({id, TempSetting});
        }
        return {};
    }

    std::string getName() const { return "SetPlayerInventoryOptionsPacket"; }

    MinecraftPacketIds getId() const { return MinecraftPacketIds::SetPlayerInventoryOptions; }
};

LL_AUTO_TYPED_INSTANCE_HOOK(
    GetHandler,
    HookPriority::Normal,
    ServerNetworkHandler,
    "?allowIncomingPacketId@ServerNetworkHandler@@UEAA_NAEBVNetworkIdentifier@@W4MinecraftPacketIds@@@Z",
    bool,
    class NetworkIdentifier const& a1,
    ::MinecraftPacketIds           packetId
) {
    TempHandler = this;
    GetHandler::unhook();
    return origin(a1, packetId);
}
LL_AUTO_TYPED_INSTANCE_HOOK(
    ReceiveInventorySettings,
    HookPriority::Normal,
    ServerNetworkHandler,
    "?allowIncomingPacketId@ServerNetworkHandler@@UEAA_NAEBVNetworkIdentifier@@W4MinecraftPacketIds@@@Z",
    bool,
    NetworkIdentifier*   a1,
    ::MinecraftPacketIds packetId
) {
    if ((int)packetId == 307) { TempNetworkId = a1; }
    return origin(a1, packetId);
}

LL_AUTO_STATIC_HOOK(
    CreatePackets,
    HookPriority::Normal,
    MinecraftPackets::createPacket,
    std::shared_ptr<class Packet>,
    MinecraftPacketIds Id
) {
    switch (Id) {
    case MinecraftPacketIds::SetPlayerInventoryOptions: {
        return std::make_shared<SetPlayerInventoryOptionsPacket>();
    };
    }
    return origin(Id);
}

LL_AUTO_TYPED_INSTANCE_HOOK(
    SendJoinPacket,
    HookPriority::Normal,
    ServerPlayer,
    "?setLocalPlayerAsInitialized@ServerPlayer@@QEAAXXZ",
    bool
) {
    long long id = this->getOrCreateUniqueID().id;
    if (PlayerInventorySettings.count(id)) {
        InventorySettings settings = PlayerInventorySettings[id];
        auto              pkt      = SetPlayerInventoryOptionsPacket(
            settings.leftTab,
            settings.rightTab,
            settings.filtering,
            settings.layout,
            settings.craftingLayout
        );
        this->sendNetworkPacket(pkt);
    } else {
        auto pkt = SetPlayerInventoryOptionsPacket(6, 2, true, 2, 2);
        this->sendNetworkPacket(pkt);
    }
    return origin();
}