#include "mc/network/packet/InventoryTransactionPacket.h"
#include "mc/world/ContainerID.h"
#include "mc/world/inventory/transaction/InventoryAction.h"
#include "mc/world/inventory/transaction/InventorySource.h"
#include "mc/world/inventory/transaction/InventoryTransaction.h"
#include "mc/world/item/NetworkItemStackDescriptor.h"

extern std::unordered_map<uint64, int> PlayerGuidMap;
int                                    NetworkProtocolVersion = 0;

bool setValuesAndMatch(ItemStack* a1, ItemStack* a2) {
    auto AuxValue = a1->getAuxValue();
    a2->setAuxValue(AuxValue);
    if (a1->hasDamageValue()) {
        auto DamageValue = a1->getDamageValue();
        a2->setDamageValue(DamageValue);
    } else {
        a2->removeDamageValue();
    }
    return a1->matches(*a2);
}

std::unordered_set<std::string> TranslateItemMap = {
    "minecraft:chest",
    "minecraft:ender_chest",
    "minecraft:stonecutter_block",
    "minecraft:trapped_chest",
    "minecraft:birch_planks",
    "minecraft:jungle_planks",
    "andesite",
    "diorite",
    "granite",
    "polished_andesite",
    "polished_diorite",
    "polished_granite",
    "stone"
};

bool isTranslateItem(std::string item) {
    if (TranslateItemMap.count(item)) { return true; }
    return false;
}

extern void Serialize_630(NetworkItemStackDescriptor NetItem, BinaryStream* bs);
LL_AUTO_TYPED_INSTANCE_HOOK(
    InventoryTransacationPacketWrite,
    HookPriority::Normal,
    InventoryTransactionPacket,
    "?write@InventoryTransactionPacket@@UEBAXAEAVBinaryStream@@@Z",
    void,
    BinaryStream& bs
) {
    if (this->mProtocolVersion >= 630) {
        int legacyRequestId = this->mLegacyRequestId.mRawId;
        bs.writeVarInt(legacyRequestId);
        if (legacyRequestId < -1 && (legacyRequestId & 1) == 0) {
            bs.writeUnsignedVarInt(this->mLegacySetItemSlots.size());
            for (auto& slot : this->mLegacySetItemSlots) {
                bs.writeByte((uchar)slot.first);
                bs.writeUnsignedVarInt(slot.second.size());
                for (auto& key : slot.second) { bs.writeByte(key); }
            }
        }
        ComplexInventoryTransaction::Type transactionType = this->mTransaction->type;
        bs.writeUnsignedVarInt((uint)transactionType);
        auto time = 0;
        for (auto& action : this->mTransaction->data.actions) { time = time + action.second.size(); }
        bs.writeUnsignedVarInt(time);
        for (auto& action : this->mTransaction->data.actions) {
            for (auto& transaction : action.second) {
                int sourceType = (int)transaction.mSource.mType;
                if (!this->mIsClientSide) {
                    if (transaction.mToItem.mValid) {
                        if (!transaction.mToItem.mItem.expired() && !transaction.mToItem.isNull()
                            && transaction.mToItem.mCount) {
                            if ((sourceType != 3 && sourceType != 2) || transaction.mSlot) {
                                transaction.mToItem.tryGetItemStackNetId();
                                transaction.mToItemDescriptor.mIncludeNetIds = true;
                            }
                        }
                    }
                }
                bs.writeUnsignedVarInt(sourceType);
                switch (sourceType) {
                case 0:
                case 99999:
                    bs.writeVarInt((int)transaction.mSource.mContainerId);
                    break;
                case 2:
                    bs.writeUnsignedVarInt((int)transaction.mSource.mFlags);
                    break;
                default: {
                    break;
                }
                }
                bs.writeUnsignedVarInt(transaction.mSlot);
                Serialize_630(transaction.mFromItemDescriptor, &bs);
                Serialize_630(transaction.mToItemDescriptor, &bs);
            }
        }
        this->mTransaction->write(bs);
    } else {
        return origin(bs);
    }
}

LL_AUTO_TYPED_INSTANCE_HOOK(
    ItemUseInventoryTransactionHandle,
    HookPriority::Normal,
    ItemUseInventoryTransaction,
    "?handle@ItemUseInventoryTransaction@@UEBA?AW4InventoryTransactionError@@AEAVPlayer@@_N@Z",
    ::InventoryTransactionError,
    Player& pl,
    bool    a1
) {
    auto ver = PlayerGuidMap[pl.getNetworkIdentifier().mGuid.g];
    if (ver != SharedConstants::NetworkProtocolVersion) {
        this->mTargetBlockId = GlobalBlockP->getBlockRuntimeIdFromOther(ver, this->mTargetBlockId);
    }
    return origin(pl, a1);
}