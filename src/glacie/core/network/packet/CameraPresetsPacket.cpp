
LL_AUTO_TYPED_INSTANCE_HOOK(
    CameraPresetsPacketWrite,
    HookPriority::Normal,
    CameraPresetsPacket,
    "?write@CameraPresetsPacket@@UEBAXAEAVBinaryStream@@@Z",
    void,
    BinaryStream* bs
) {
    if (this->mProtocolVersion >= 618) {
        origin(bs);
    } else {
        auto nbt =
            CompoundTag::fromSnbt(R"({"properties":[{"name":"minecraft:has_nectar","type":2}],"type":"minecraft:bee")");
        bs->writeCompoundTag(*nbt.get());
    }
}