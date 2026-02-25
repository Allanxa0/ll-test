
LL_AUTO_TYPED_INSTANCE_HOOK(
    UpdateSubBlockPacketWrite,
    HookPriority::Normal,
    UpdateSubChunkBlocksPacket,
    "?write@UpdateSubChunkBlocksPacket@@UEBAXAEAVBinaryStream@@@Z",
    void,
    BinaryStream* bs
) {
    if (this->mProtocolVersion != SharedConstants::NetworkProtocolVersion) {
        bs->writeVarInt(this->mSubChunkBlockPosition.x);
        bs->writeUnsignedVarInt(this->mSubChunkBlockPosition.y);
        bs->writeVarInt(this->mSubChunkBlockPosition.z);
        bs->writeUnsignedVarInt(this->mBlocksChanged.mStandards.size());
        for (auto& info : this->mBlocksChanged.mStandards) {
            bs->writeVarInt(info.mPos.x);
            bs->writeUnsignedVarInt(info.mPos.y);
            bs->writeVarInt(info.mPos.z);
            bs->writeUnsignedVarInt(GlobalBlockP->getBlockRuntimeIdFromMain(this->mProtocolVersion, info.mRuntimeId));
            bs->writeUnsignedVarInt(info.mUpdateFlags);
            bs->writeVarInt64(info.mSyncMessage.mEntityUniqueID.id);
            bs->writeUnsignedVarInt((int)info.mSyncMessage.mMessage);
        }
        bs->writeUnsignedVarInt(this->mBlocksChanged.mExtras.size());
        for (auto& info : this->mBlocksChanged.mExtras) {
            bs->writeVarInt(info.mPos.x);
            bs->writeUnsignedVarInt(info.mPos.y);
            bs->writeVarInt(info.mPos.z);
            bs->writeUnsignedVarInt(GlobalBlockP->getBlockRuntimeIdFromMain(this->mProtocolVersion, info.mRuntimeId));
            bs->writeUnsignedVarInt(info.mUpdateFlags);
            bs->writeVarInt64(info.mSyncMessage.mEntityUniqueID.id);
            bs->writeUnsignedVarInt((int)info.mSyncMessage.mMessage);
        }
    } else {
        origin(bs);
    }
}
