
extern GlobalItemData* GlobalItemDataP;

std::unordered_set<std::string> NewItem_630 = {
    "minecraft:andesite",
    "minecraft:diorite",
    "minecraft:granite",
    "minecraft:polished_andesite",
    "minecraft:polished_diorite",
    "minecraft:polished_granite",
    "minecraft:stone",
    "minecraft:oak_planks",
    "minecraft:birch_planks",
    "minecraft:jungle_planks",
    "minecraft:acacia_planks",
    "minecraft:dark_oak_planks"
};

LL_AUTO_TYPED_INSTANCE_HOOK(
    StartGamePacketWrite,
    HookPriority::Normal,
    StartGamePacket,
    "?write@StartGamePacket@@UEBAXAEAVBinaryStream@@@Z",
    void,
    BinaryStream* bs
) {
    this->mServerBlockTypeRegistryChecksum = 0;
    if (this->mProtocolVersion == 630) {
        for (auto& item : this->mItemData) {
            if (item.mName.str == "minecraft:planks") {
                item.mName.str = "minecraft:oak_planks";
                item.mId       = 5;
            }
        }
        for (auto& item : GlobalItemDataP->getItemData(630)) {
            if (NewItem_630.count(item.mName.str)) {
                this->mItemData.push_back(ItemData(HashedString(item.mName.str), item.mId, item.mIsComponentBased));
            }
        }
    }
    return origin(bs);
}

LL_AUTO_TYPED_INSTANCE_HOOK(
    AddItemActorPacketWrite,
    HookPriority::Normal,
    AddItemActorPacket,
    "?write@AddItemActorPacket@@UEBAXAEAVBinaryStream@@@Z",
    void,
    BinaryStream* bs
) {
    if (this->mProtocolVersion != SharedConstants::NetworkProtocolVersion) {
        this->mItem.mBlockRuntimeId =
            GlobalBlockP->getBlockRuntimeIdFromMain(this->mProtocolVersion, this->mItem.mBlockRuntimeId);
    }
    return origin(bs);
}
