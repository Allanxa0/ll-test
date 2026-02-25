
extern std::unordered_map<uint64, int> PlayerGuidMap;

LL_AUTO_TYPED_INSTANCE_HOOK(
    CameraInstructionPacketSend,
    HookPriority::Normal,
    LoopbackPacketSender,
    "?sendToClient@LoopbackPacketSender@@UEAAXPEBVUserEntityIdentifierComponent@@AEBVPacket@@@Z",
    void,
    UserEntityIdentifierComponent* a2,
    Packet*                        a3
) {
    auto protocol = PlayerGuidMap[a2->mNetworkId.mGuid.g];
    if (protocol < 618) {
        if (a3->getId() == MinecraftPacketIds::CameraInstruction) {
            auto pkt = TextPacket::createTranslated(
                "%packdiscoveryerror.invalid_capability_value",
                {"%createWorldScreen.experimentalCameras"}
            );
            return origin(a2, (Packet*)&pkt);
        }
    }
    return origin(a2, a3);
}