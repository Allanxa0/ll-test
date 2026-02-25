
LL_AUTO_TYPED_INSTANCE_HOOK(
    LoginPacketRead,
    HookPriority::Normal,
    LoginPacket,
    "?_read@LoginPacket@@EEAA?AV?$Result@XVerror_code@std@@@Bedrock@@AEAVReadOnlyBinaryStream@@@Z",
    Bedrock::Result<void>,
    ReadOnlyBinaryStream* stream
) {
    this->mClientNetworkVersion = SharedConstants::NetworkProtocolVersion;
    stream->getSignedBigEndianInt();
    std::string ConnectionRequestStr;
    stream->getString(ConnectionRequestStr);
    auto cr                  = ConnectionRequest::fromString(ConnectionRequestStr);
    this->mConnectionRequest = std::make_unique<ConnectionRequest>(cr);
    return {};
}
