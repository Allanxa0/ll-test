extern void Serialize_630(NetworkItemStackDescriptor NetItem, BinaryStream* bs);
LL_AUTO_TYPED_INSTANCE_HOOK(
    InventorySlotPacketWrite,
    HookPriority::Normal,
    InventorySlotPacket,
    "?write@InventorySlotPacket@@UEBAXAEAVBinaryStream@@@Z",
    void,
    BinaryStream& bs
) {
    if (this->mProtocolVersion >= 630) {
        bs.writeUnsignedVarInt((char)this->mInventoryId);
        bs.writeUnsignedVarInt(this->mSlot);
        this->mItem.tryGetServerNetId();
        if(this->mItem.isValid(true)&&this->mItem.getItem()&&!this->mItem.isNull()&&this->mItem.getStackSize()>0){
            this->mItem.mIncludeNetIds=true;
        }
        Serialize_630(this->mItem, &bs);
    } else {
        origin(bs);
    }
}