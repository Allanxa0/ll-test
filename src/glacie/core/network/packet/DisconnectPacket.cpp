
LL_AUTO_TYPED_INSTANCE_HOOK(
    DisconnectPacketWrite,
    HookPriority::Normal,
    DisconnectPacket,
    "?write@DisconnectPacket@@UEBAXAEAVBinaryStream@@@Z",
    void,
    BinaryStream* bs
) {
    if (this->mProtocolVersion >= 622) {
        bs->writeVarInt(-1);
        bs->writeBool(this->mSkipMessage);
        bs->writeString(this->mMessage);
    } else {
        origin(bs);
    }
}