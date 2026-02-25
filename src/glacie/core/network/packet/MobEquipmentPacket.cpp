#include "mc/network/packet/MobEquipmentPacket.h"
#include "glacie/api/memory/MemoryUtils.h"
#include "glacie/api/service/GlobalService.h"
#include "mc/network/MinecraftPacketIds.h"
#include "mc/common/SubClientId.h"
#include "mc/network/MinecraftPackets.h"
#include "mc/world/item/NetworkItemStackDescriptor.h"
inline void Serialize_630(NetworkItemStackDescriptor NetItem, BinaryStream* bs) {
    auto                                id           = NetItem.getId();
    auto                                StackSize    = NetItem.getStackSize();
    std::shared_ptr<class ItemRegistry> itemRegistry = ItemRegistryManager::getItemRegistry()._lockRegistry();
    WeakPtr<class Item>                 item         = itemRegistry->getItem(id);
    short                               auxValue     = 0;
    Item*                               ptr          = nullptr;

    auto idaux = NetItem.getIdAux();
    if (!item.expired()) ptr = item.get();
    if (NetItem.isValid(true) && ptr) {
        switch (idaux) {
        case 327681: {
            bs->writeVarInt(-739);
            bs->writeUnsignedShort(StackSize);
            bs->writeUnsignedVarInt(0);
            break;
        }
        case 327682: {
            bs->writeVarInt(-740);
            bs->writeUnsignedShort(StackSize);
            bs->writeUnsignedVarInt(0);
            break;
        }
        case 327683: {
            bs->writeVarInt(-741);
            bs->writeUnsignedShort(StackSize);
            bs->writeUnsignedVarInt(0);
            break;
        }
        case 327684: {
            bs->writeVarInt(-742);
            bs->writeUnsignedShort(StackSize);
            bs->writeUnsignedVarInt(0);
            break;
        }
        case 327685: {
            bs->writeVarInt(-743);
            bs->writeUnsignedShort(StackSize);
            bs->writeUnsignedVarInt(0);
            break;
        }
        case 65537: {
            bs->writeVarInt(-590);
            bs->writeUnsignedShort(StackSize);
            bs->writeUnsignedVarInt(0);
            break;
        }
        case 65539: {
            bs->writeVarInt(-592);
            bs->writeUnsignedShort(StackSize);
            bs->writeUnsignedVarInt(0);
            break;
        }
        case 65541: {
            bs->writeVarInt(-594);
            bs->writeUnsignedShort(StackSize);
            bs->writeUnsignedVarInt(0);
            break;
        }
        case 65538: {
            bs->writeVarInt(-591);
            bs->writeUnsignedShort(StackSize);
            bs->writeUnsignedVarInt(0);
            break;
        }
        case 65540: {
            bs->writeVarInt(-593);
            bs->writeUnsignedShort(StackSize);
            bs->writeUnsignedVarInt(0);
            break;
        }
        case 65542: {
            bs->writeVarInt(-595);
            bs->writeUnsignedShort(StackSize);
            bs->writeUnsignedVarInt(0);
            break;
        }
        default: {
            bs->writeVarInt(id);
            bs->writeUnsignedShort(StackSize);
            if (!NetItem.getBlock()) { auxValue = NetItem.getAuxValue(); }
            bs->writeUnsignedVarInt(auxValue);
        }
        }

        bs->writeBool(NetItem.mIncludeNetIds);
        if (NetItem.mIncludeNetIds) {
            NetItem.mNetIdVariant.serialize(*bs);
        }
        bs->writeVarInt(GlobalBlockP->getBlockRuntimeIdFromMain(630, NetItem.mBlockRuntimeId));
        bs->writeString(NetItem.mUserDataBuffer);
    } else {
        bs->writeVarInt(0);
    }
}
LL_AUTO_TYPED_INSTANCE_HOOK(
    MobEquipmentPacketWrite,
    HookPriority::Normal,
    MobEquipmentPacket,
    "?write@MobEquipmentPacket@@UEBAXAEAVBinaryStream@@@Z",
    void,
    BinaryStream& bs
) {
    if (this->mProtocolVersion == 630) {
        bs.writeUnsignedVarInt64(this->mRuntimeId.id);
        Serialize_630(this->mItem, &bs);
        bs.writeUnsignedChar(this->mSlotByte);
        bs.writeUnsignedChar(this->mSelectedSlotByte);
        bs.writeUnsignedChar(this->mContainerIdByte);
    } else {
        origin(bs);
    }
}

LL_AUTO_TYPED_INSTANCE_HOOK(
    MobEquipmentPacketHandle,
    HookPriority::Normal,
    ServerNetworkHandler,
    "?handle@ServerNetworkHandler@@UEAAXAEBVNetworkIdentifier@@AEBVMobEquipmentPacket@@@Z",
    void,
    NetworkIdentifier*  a1,
    MobEquipmentPacket* a2
) {
    if (a2->mProtocolVersion == 630) {
        auto ac                   = ll::Global<Level>->getRuntimeEntity(a2->mRuntimeId);
        a2->mItem.mBlockRuntimeId = GlobalBlockP->getBlockRuntimeIdFromOther(630, a2->mItem.mBlockRuntimeId);
        if (!ac->isPlayer()) { return origin(a1, a2); }
        auto pl   = (Player*)ac;
        auto Item = pl->getInventory().getItem(a2->mSlot);
        auto pkt  = MobEquipmentPacket(a2->mRuntimeId, Item, a2->mSlot, a2->mSelectedSlot, a2->mContainerId);
        return origin(a1, &pkt);
    }
    return origin(a1, a2);
}