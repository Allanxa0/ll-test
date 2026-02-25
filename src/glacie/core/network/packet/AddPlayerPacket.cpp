
extern void Serialize_630(NetworkItemStackDescriptor NetItem, BinaryStream* bs);
LL_AUTO_TYPED_INSTANCE_HOOK(
    AddPlayerPacketWrite,
    HookPriority::Normal,
    AddPlayerPacket,
    "?write@AddPlayerPacket@@UEBAXAEAVBinaryStream@@@Z",
    void,
    BinaryStream& bs
) {
    if (this->mProtocolVersion == 630) {
        bs.writeUnsignedInt64(this->mUuid.a);
        bs.writeUnsignedInt64(this->mUuid.b);
        bs.writeString(this->mName);
        bs.writeUnsignedVarInt64(this->mRuntimeId.id);
        bs.writeString(this->mPlatformOnlineId);
        bs.writeType(this->mPos);
        bs.writeType(this->mVelocity);
        bs.writeFloat(this->mRot.x);
        bs.writeFloat(this->mRot.y);
        bs.writeFloat(this->mYHeadRot);
        Serialize_630(this->mCarriedItem, &bs);
        bs.writeVarInt((int)this->mPlayerGameType);
        auto mEntityData = (SynchedActorDataEntityWrapper*)this->mEntityData;
        if (mEntityData) {
            auto packedData = mEntityData->packAll();
            bs.writeType(packedData);
        } else {
            bs.writeType(this->mUnpack);
        }
        bs.writeType(this->mSynchedProperties);
        auto AbilitiesData = SerializedAbilitiesData::SerializedAbilitiesData(this->mEntityId, this->mAbilities);
        bs.writeType(AbilitiesData);
        bs.writeUnsignedVarInt(this->mLinks.size());
        for (auto& link : this->mLinks) { bs.writeType(link); }
        bs.writeString(this->mDeviceId);
        bs.writeUnsignedInt((int)this->mBuildPlatform);
    } else {
        origin(bs);
    }
}