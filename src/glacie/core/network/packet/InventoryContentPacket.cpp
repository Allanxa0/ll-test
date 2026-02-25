#include "mc/world/item/NetworkItemStackDescriptor.h"

extern void Serialize_630(NetworkItemStackDescriptor NetItem, BinaryStream* bs);

LL_AUTO_TYPED_INSTANCE_HOOK(
    InventoryContentPacketWrite,
    HookPriority::Normal,
    InventoryContentPacket,
    "?write@InventoryContentPacket@@UEBAXAEAVBinaryStream@@@Z",
    void,
    BinaryStream* bs
) {
    if (this->mProtocolVersion >= 630) {
        bs->writeUnsignedVarInt((char)this->mInventoryId);
        bs->writeUnsignedVarInt(this->mSlots.size());
        for (auto& i : this->mSlots) {
            i.mIncludeNetIds = true;
            Serialize_630(i, bs);
        }
    } else {
        origin(bs);
    }
}