

LL_AUTO_TYPED_INSTANCE_HOOK(
    ResourcePacksInfoPacketWrite,
    HookPriority::Normal,
    ResourcePacksInfoPacket,
    "?write@ResourcePacksInfoPacket@@UEBAXAEAVBinaryStream@@@Z",
    void,
    BinaryStream* bs
) {
    if (this->mProtocolVersion >= 618) {
        origin(bs);
    } else {
        bs->writeBool(this->mData.mResourcePackRequired);
        bs->writeBool(this->mData.mHasScripts);
        bs->writeBool(this->mData.mForceServerPacksEnabled);
        bs->writeUnsignedShort(this->mData.mBehaviorPacks.size());
        for (auto bp : this->mData.mBehaviorPacks) {
            bs->writeString(bp.mPackIdVersion.mId.asString());
            bs->writeString(bp.mPackIdVersion.mVersion.asString());
            bs->writeUnsignedInt64(bp.mPackSize);
            bs->writeString(bp.mContentKey);
            bs->writeString(bp.mSubpackName);
            bs->writeString(bp.mContentIdentity.asString());
            bs->writeBool(bp.mHasScripts);
        }
        bs->writeUnsignedShort(this->mData.mResourcePacks.size());
        for (auto rp : this->mData.mResourcePacks) {
            bs->writeString(rp.mPackIdVersion.mId.asString());
            bs->writeString(rp.mPackIdVersion.mVersion.asString());
            bs->writeUnsignedInt64(rp.mPackSize);
            bs->writeString(rp.mContentKey);
            bs->writeString(rp.mSubpackName);
            bs->writeString(rp.mContentIdentity.asString());
            bs->writeBool(rp.mHasScripts);
            bs->writeBool(rp.mIsRayTracingCapable); 
        }
    }
}