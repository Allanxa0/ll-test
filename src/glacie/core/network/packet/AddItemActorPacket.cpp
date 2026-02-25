
#include "mc/world/item/NetworkItemStackDescriptor.h"
extern void Serialize_630(NetworkItemStackDescriptor NetItem, BinaryStream* bs);
LL_AUTO_TYPED_INSTANCE_HOOK(
    AddItemActorPacketWrite,
    HookPriority::Normal,
    AddItemActorPacket,
    "?write@AddItemActorPacket@@UEBAXAEAVBinaryStream@@@Z",
    void,
    BinaryStream& bs
) {
    if (this->mProtocolVersion == 630) {
        bs.writeVarInt64(this->mId.id);
        bs.writeUnsignedVarInt64(this->mRuntimeId.id);
        Serialize_630(this->mItem, &bs);
        bs.writeFloat(this->mPos.x);
        bs.writeFloat(this->mPos.y);
        bs.writeFloat(this->mPos.z);
        bs.writeFloat(this->mVelocity.x);
        bs.writeFloat(this->mVelocity.y);
        bs.writeFloat(this->mVelocity.z);
        if (this->mEntityData) {
            bs.writeType(this->mEntityData->packAll());
        } else {
            bs.writeType(this->mData);
        }
        bs.writeBool(this->mIsFromFishing);

    } else {
        return origin(bs);
    }
}