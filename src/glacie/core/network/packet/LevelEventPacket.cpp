
LL_AUTO_TYPED_INSTANCE_HOOK(
    LevelEventPacketWrite,
    HookPriority::Normal,
    LevelEventPacket,
    "?write@LevelEventPacket@@UEBAXAEAVBinaryStream@@@Z",
    void,
    BinaryStream* bs
) {
    if (this->mProtocolVersion != SharedConstants::NetworkProtocolVersion) {
        if ((this->mEventId >= 3603 && this->mEventId <= 3608) || this->mEventId == 2001) {
            this->mData = GlobalBlockP->getBlockRuntimeIdFromMain(this->mProtocolVersion, this->mData);
        }
    }
    return origin(bs);
}