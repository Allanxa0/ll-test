
LL_AUTO_TYPED_INSTANCE_HOOK(
    ShowStoreOfferPacketWrite,
    HookPriority::Normal,
    ShowStoreOfferPacket,
    "?write@ShowStoreOfferPacket@@UEBAXAEAVBinaryStream@@@Z",
    void,
    BinaryStream* bs
) {
    if (this->mProtocolVersion > 622) {
        bs->writeString(this->mOfferId);
        bs->writeBool(this->mShowAll);
        bs->writeUnsignedVarInt(0);
    } else {
        origin(bs);
    }
}