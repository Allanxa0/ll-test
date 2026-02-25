
extern std::unordered_map<uint64, int> PlayerGuidMap;

LL_AUTO_TYPED_INSTANCE_HOOK(
    UpdateBlockPacketWrite,
    HookPriority::Normal,
    UpdateBlockPacket,
    "?write@UpdateBlockPacket@@UEBAXAEAVBinaryStream@@@Z",
    void,
    BinaryStream* bs
) {
    if (this->mProtocolVersion != SharedConstants::NetworkProtocolVersion) {
        bs->writeVarInt(this->mPos.x);
        bs->writeUnsignedVarInt(this->mPos.y);
        bs->writeVarInt(this->mPos.z);
        bs->writeUnsignedVarInt(GlobalBlockP->getBlockRuntimeIdFromMain(this->mProtocolVersion, this->mRuntimeId));
        bs->writeUnsignedVarInt((int)this->mUpdateFlags);
        bs->writeUnsignedVarInt((int)this->mLayer);
    } else {
        origin(bs);
    }
}
