#include "mc/network/packet/CreativeContentPacket.h"
#include "glacie/api/utils/Base64.h"
#include "mc/world/item/Item.h"

namespace CreativeContentPacketSerialize {

void Serialize_594(CreativeContentPacket* pkt, BinaryStream* stream) {
    if (!pkt->mWriteEntries.empty()) {
        stream->writeUnsignedVarInt(pkt->mWriteEntries.size());
        for (auto& key : pkt->mWriteEntries) {
            CreativeItemNetId const& CreativeNetId = key.getCreativeNetId();
            stream->writeUnsignedVarInt(CreativeNetId.mRawId);
            ItemInstance const&                 ItemInstance = key.getItemInstance();
            NetworkItemInstanceDescriptor       networkitem(ItemInstance);
            int16                               id           = networkitem.getId();
            int16                               stackSize    = networkitem.getStackSize();
            std::shared_ptr<class ItemRegistry> itemRegistry = ItemRegistryManager::getItemRegistry()._lockRegistry();
            WeakPtr<class Item>                 item         = itemRegistry->getItem(id);
            short                               auxValue     = 0;
            Item*                               ptr          = nullptr;
            if (!item.expired()) ptr = item.get();
            if (networkitem.isValid(true) && ptr) {
                if (id == -646) {
                    stream->writeVarInt(160);
                    stream->writeUnsignedShort(stackSize);
                    stream->writeUnsignedVarInt(4);
                    stream->writeVarInt(
                        GlobalBlockP->getBlockRuntimeIdFromMain(pkt->mProtocolVersion, networkitem.mBlockRuntimeId)
                    );
                    stream->writeString(networkitem.mUserDataBuffer);
                } else {
                    stream->writeVarInt(id);
                    stream->writeUnsignedShort(stackSize);
                    if (!networkitem.getBlock()) { auxValue = networkitem.getAuxValue(); }
                    stream->writeUnsignedVarInt(auxValue);
                    stream->writeVarInt(
                        GlobalBlockP->getBlockRuntimeIdFromMain(pkt->mProtocolVersion, networkitem.mBlockRuntimeId)
                    );
                    stream->writeString(networkitem.mUserDataBuffer);
                }

            } else {
                stream->writeVarInt(0);
            }
        }
    }
}

void Serialize_630(CreativeContentPacket* pkt, BinaryStream* stream) {
    auto list = GlobalItemDataP->getContent(630);
    if (!pkt->mWriteEntries.empty()) {
        stream->writeUnsignedVarInt(pkt->mWriteEntries.size());
        for (auto& key : pkt->mWriteEntries) {
            CreativeItemNetId const& CreativeNetId = key.getCreativeNetId();
            stream->writeUnsignedVarInt(CreativeNetId.mRawId);
            ItemInstance const&                 ItemInstance = key.getItemInstance();
            NetworkItemInstanceDescriptor       networkitem(ItemInstance);
            int16                               id           = networkitem.getId();
            int16                               stackSize    = networkitem.getStackSize();
            std::shared_ptr<class ItemRegistry> itemRegistry = ItemRegistryManager::getItemRegistry()._lockRegistry();
            WeakPtr<class Item>                 item         = itemRegistry->getItem(id);
            short                               auxValue     = 0;
            Item*                               ptr          = nullptr;
            if (!item.expired()) ptr = item.get();
            if (networkitem.isValid(true) && ptr) {
                if (!networkitem.getBlock()) { auxValue = networkitem.getAuxValue(); }
                switch (CreativeNetId.mRawId) {
                case 2: {
                    stream->writeVarInt(-739);
                    stream->writeUnsignedShort(stackSize);
                    stream->writeUnsignedVarInt(0);
                    break;
                }
                case 3: {
                    stream->writeVarInt(-740);
                    stream->writeUnsignedShort(stackSize);
                    stream->writeUnsignedVarInt(0);
                    break;
                }
                case 4: {
                    stream->writeVarInt(-741);
                    stream->writeUnsignedShort(stackSize);
                    stream->writeUnsignedVarInt(0);
                    break;
                }
                case 5: {
                    stream->writeVarInt(-742);
                    stream->writeUnsignedShort(stackSize);
                    stream->writeUnsignedVarInt(0);
                    break;
                }
                case 6: {
                    stream->writeVarInt(-743);
                    stream->writeUnsignedShort(stackSize);
                    stream->writeUnsignedVarInt(0);
                    break;
                }
                case 442: {
                    stream->writeVarInt(-590);
                    stream->writeUnsignedShort(stackSize);
                    stream->writeUnsignedVarInt(0);
                    break;
                }
                case 443: {
                    stream->writeVarInt(-592);
                    stream->writeUnsignedShort(stackSize);
                    stream->writeUnsignedVarInt(0);
                    break;
                }
                case 444: {
                    stream->writeVarInt(-594);
                    stream->writeUnsignedShort(stackSize);
                    stream->writeUnsignedVarInt(0);
                    break;
                }
                case 447: {
                    stream->writeVarInt(-591);
                    stream->writeUnsignedShort(stackSize);
                    stream->writeUnsignedVarInt(0);
                    break;
                }
                case 448: {
                    stream->writeVarInt(-593);
                    stream->writeUnsignedShort(stackSize);
                    stream->writeUnsignedVarInt(0);
                    break;
                }
                case 449: {
                    stream->writeVarInt(-595);
                    stream->writeUnsignedShort(stackSize);
                    stream->writeUnsignedVarInt(0);
                    break;
                }

                default: {
                    stream->writeVarInt(id);
                    stream->writeUnsignedShort(stackSize);
                    stream->writeUnsignedVarInt(auxValue);
                }
                }
                stream->writeVarInt(
                    GlobalBlockP->getBlockRuntimeIdFromMain(pkt->mProtocolVersion, networkitem.mBlockRuntimeId)
                );
                stream->writeString(networkitem.mUserDataBuffer);
            } else {
                stream->writeVarInt(0);
            }
        }
    }
}

} // namespace CreativeContentPacketSerialize

using namespace CreativeContentPacketSerialize;

LL_AUTO_TYPED_INSTANCE_HOOK(
    CreativeContentPacketWrite,
    HookPriority::Normal,
    CreativeContentPacket,
    "?write@CreativeContentPacket@@UEBAXAEAVBinaryStream@@@Z",
    void,
    BinaryStream* stream
) {
    switch (this->mProtocolVersion) {
    case 594:
        Serialize_594(this, stream);
        break;
    case 622:
        origin(stream);
        break;
    case 630:
        Serialize_630(this, stream);
        break;
    default:
        origin(stream);
        break;
    }
};

LL_AUTO_STATIC_HOOK(
    _handleRequestData,
    HookPriority::Normal,
    "?_handleRequestData@ItemStackNetManagerServer@@AEAAXAEAV?$vector@UItemStackResponseInfo@@V?$allocator@"
    "UItemStackResponseInfo@@@std@@@std@@PEBVItemStackRequestData@@@Z",
    void,
    ItemStackNetManagerServer*                 a1,
    std::vector<struct ItemStackResponseInfo>& a2,
    class ItemStackRequestData const*          a3
) {
    auto& list = const_cast<std::vector<std::unique_ptr<class ItemStackRequestAction>>&>(a3->getActions());
    for (auto i = 0; i < list.size(); ++i) {
        if (list[i].get() == nullptr) {

            list.erase(list.begin() + i);
        } else {
        }
    }
    return origin(a1, a2, a3);
};
