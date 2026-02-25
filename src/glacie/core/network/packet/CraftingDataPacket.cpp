#include "glacie/api/memory/MemoryUtils.h"
#include "mc/world/item/ItemDescriptor.h"
#include "mc/world/item/NetworkItemStackDescriptor.h"
#include "mc/world/item/crafting/ShapedRecipe.h"

void InsSerialize_630(ItemInstance ItemInstance, BinaryStream* stream) {
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
        switch (networkitem.getIdAux()) {
        case 327681: {
            stream->writeVarInt(-739);
            stream->writeUnsignedShort(stackSize);
            stream->writeUnsignedVarInt(0);
            break;
        }
        case 327682: {
            stream->writeVarInt(-740);
            stream->writeUnsignedShort(stackSize);
            stream->writeUnsignedVarInt(0);
            break;
        }
        case 327683: {
            stream->writeVarInt(-741);
            stream->writeUnsignedShort(stackSize);
            stream->writeUnsignedVarInt(0);
            break;
        }
        case 327684: {
            stream->writeVarInt(-742);
            stream->writeUnsignedShort(stackSize);
            stream->writeUnsignedVarInt(0);
            break;
        }
        case 327685: {
            stream->writeVarInt(-743);
            stream->writeUnsignedShort(stackSize);
            stream->writeUnsignedVarInt(0);
            break;
        }
        case 65537: {
            stream->writeVarInt(-590);
            stream->writeUnsignedShort(stackSize);
            stream->writeUnsignedVarInt(0);
            break;
        }
        case 65539: {
            stream->writeVarInt(-592);
            stream->writeUnsignedShort(stackSize);
            stream->writeUnsignedVarInt(0);
            break;
        }
        case 65541: {
            stream->writeVarInt(-594);
            stream->writeUnsignedShort(stackSize);
            stream->writeUnsignedVarInt(0);
            break;
        }
        case 65538: {
            stream->writeVarInt(-591);
            stream->writeUnsignedShort(stackSize);
            stream->writeUnsignedVarInt(0);
            break;
        }
        case 65540: {
            stream->writeVarInt(-593);
            stream->writeUnsignedShort(stackSize);
            stream->writeUnsignedVarInt(0);
            break;
        }
        case 65542: {
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
        stream->writeVarInt(GlobalBlockP->getBlockRuntimeIdFromMain(630, networkitem.mBlockRuntimeId));
        stream->writeString(networkitem.mUserDataBuffer);
    } else {
        stream->writeVarInt(0);
    }
}
LL_AUTO_TYPED_INSTANCE_HOOK(
    CraftingDataPacketWrite,
    HookPriority::Normal,
    CraftingDataPacket,
    "?write@CraftingDataPacket@@UEBAXAEAVBinaryStream@@@Z",
    void,
    BinaryStream& bs
) {
    if (this->mProtocolVersion >= 630) {
        bs.writeUnsignedVarInt(this->mCraftingEntries.size());
        for (auto& recipe : this->mCraftingEntries) {
            switch ((int)recipe.mEntryType) {
            case 0: {
                bs.writeVarInt((int)recipe.mEntryType);
                bs.writeString(recipe.mRecipe->mRecipeId);
                bs.writeUnsignedVarInt(recipe.mRecipe->mMyIngredients.size());
                for (auto& Ingredients : recipe.mRecipe->mMyIngredients) {
                    Ingredients._resolve();
                    if (!Ingredients.isNull()) {
                        bs.writeUnsignedChar((char)Ingredients.mImpl->getType());
                        if (Ingredients.mImpl->getType() == ItemDescriptor::InternalType::Default) {
                            auto DItem = Ingredients.mImpl->getItem().mItem;
                            if (DItem) {
                                auto idaux0 = Ingredients.getIdAux();
                                auto id     = DItem->getId();
                                switch (idaux0) {
                                case 327681: {
                                    bs.writeUnsignedShort(-739);
                                    break;
                                }
                                case 327682: {
                                    bs.writeUnsignedShort(-740);
                                    break;
                                }
                                case 327683: {
                                    bs.writeUnsignedShort(-741);
                                    break;
                                }
                                case 327684: {
                                    bs.writeUnsignedShort(-742);
                                    break;
                                }
                                case 327685: {
                                    bs.writeUnsignedShort(-743);
                                    break;
                                }
                                case 65537: {
                                    bs.writeUnsignedShort(-590);
                                    break;
                                }
                                case 65539: {
                                    bs.writeUnsignedShort(-592);
                                    break;
                                }
                                case 65541: {
                                    bs.writeUnsignedShort(-594);
                                    break;
                                }
                                case 65538: {
                                    bs.writeUnsignedShort(-591);
                                    break;
                                }
                                case 65540: {
                                    bs.writeUnsignedShort(-593);
                                    break;
                                }
                                case 65542: {
                                    bs.writeUnsignedShort(-595);
                                    break;
                                }

                                default: {
                                    bs.writeUnsignedShort(id);
                                }
                                }
                                auto si = Ingredients.mImpl->getItem().mAuxValue;
                                bs.writeUnsignedShort(si);
                            } else {
                                bs.writeUnsignedShort(0);
                            }
                        } else {
                            Ingredients.mImpl->serialize(bs);
                        }
                    } else {
                        bs.writeUnsignedChar(0);
                    }
                    bs.writeVarInt(Ingredients.mStackSize);
                }
                bs.writeUnsignedVarInt(recipe.mRecipe->getResultItem().size());
                for (auto& result : recipe.mRecipe->getResultItem()) { InsSerialize_630(result, &bs); }
                bs.writeUnsignedInt64(recipe.mRecipe->mMyId.a);
                bs.writeUnsignedInt64(recipe.mRecipe->mMyId.b);
                bs.writeString(recipe.mRecipe->getTag().getString());
                bs.writeVarInt(recipe.mRecipe->mPriority);
                auto NetId = recipe.mRecipe->mRecipeNetId;
                bs.writeUnsignedVarInt(NetId.mRawId);
                break;
            }
            case 1: {
                bs.writeVarInt((int)recipe.mEntryType);
                bs.writeString(recipe.mRecipe->mRecipeId);
                bs.writeVarInt(recipe.mRecipe->mWidth);
                bs.writeVarInt(recipe.mRecipe->mHeight);
                for (int z = 0; z < recipe.mRecipe->mHeight; ++z) {
                    for (int x = 0; x < recipe.mRecipe->mWidth; ++x) {
                        recipe.mRecipe->getIngredient(x, z)._resolve();
                        if (!recipe.mRecipe->getIngredient(x, z).isNull()) {
                            bs.writeUnsignedChar((char)recipe.mRecipe->getIngredient(x, z).mImpl->getType());
                            if (recipe.mRecipe->getIngredient(x, z).mImpl->getType()
                                == ItemDescriptor::InternalType::Default) {
                                auto DItem = recipe.mRecipe->getIngredient(x, z).mImpl->getItem().mItem;
                                if (DItem) {
                                    auto idaux0 = recipe.mRecipe->getIngredient(x, z).getIdAux();
                                    auto id     = DItem->getId();
                                    switch (idaux0) {
                                    case 327681: {
                                        bs.writeUnsignedShort(-739);
                                        break;
                                    }
                                    case 327682: {
                                        bs.writeUnsignedShort(-740);
                                        break;
                                    }
                                    case 327683: {
                                        bs.writeUnsignedShort(-741);
                                        break;
                                    }
                                    case 327684: {
                                        bs.writeUnsignedShort(-742);
                                        break;
                                    }
                                    case 327685: {
                                        bs.writeUnsignedShort(-743);
                                        break;
                                    }
                                    case 65537: {
                                        bs.writeUnsignedShort(-590);
                                        break;
                                    }
                                    case 65539: {
                                        bs.writeUnsignedShort(-592);
                                        break;
                                    }
                                    case 65541: {
                                        bs.writeUnsignedShort(-594);
                                        break;
                                    }
                                    case 65538: {
                                        bs.writeUnsignedShort(-591);
                                        break;
                                    }
                                    case 65540: {
                                        bs.writeUnsignedShort(-593);
                                        break;
                                    }
                                    case 65542: {
                                        bs.writeUnsignedShort(-595);
                                        break;
                                    }

                                    default: {
                                        bs.writeUnsignedShort(id);
                                    }
                                    }
                                    auto si = recipe.mRecipe->getIngredient(x, z).mImpl->getItem().mAuxValue;
                                    bs.writeUnsignedShort(si);
                                } else {
                                    bs.writeUnsignedShort(0);
                                }
                            } else {
                                recipe.mRecipe->getIngredient(x, z).mImpl->serialize(bs);
                            }
                        } else {
                            bs.writeUnsignedChar(0);
                        }
                        bs.writeVarInt(recipe.mRecipe->getIngredient(x, z).mStackSize);
                    }
                }
                bs.writeUnsignedVarInt(recipe.mRecipe->getResultItem().size());
                for (auto& result : recipe.mRecipe->getResultItem()) { InsSerialize_630(result, &bs); }
                bs.writeUnsignedInt64(recipe.mRecipe->mMyId.a);
                bs.writeUnsignedInt64(recipe.mRecipe->mMyId.b);
                bs.writeString(recipe.mRecipe->getTag().getString());
                bs.writeVarInt(recipe.mRecipe->mPriority);
                auto NetId = recipe.mRecipe->mRecipeNetId;
                bs.writeUnsignedVarInt(NetId.mRawId);
                break;
            }
            default:
                recipe.write(bs);
                break;
            }
        }
        bs.writeUnsignedVarInt(this->mPotionMixEntries.size());
        for (auto& PotionMixEntry : mPotionMixEntries) {
            bs.writeVarInt(PotionMixEntry.fromItemId);
            bs.writeVarInt(PotionMixEntry.fromItemAux);
            bs.writeVarInt(PotionMixEntry.reagentItemId);
            bs.writeVarInt(PotionMixEntry.reagentItemAux);
            bs.writeVarInt(PotionMixEntry.toItemId);
            bs.writeVarInt(PotionMixEntry.toItemAux);
        }
        bs.writeUnsignedVarInt(this->mContainerMixEntries.size());
        for (auto& ContainerMixEntry : mContainerMixEntries) {
            bs.writeVarInt(ContainerMixEntry.fromItemId);
            bs.writeVarInt(ContainerMixEntry.reagentItemId);
            bs.writeVarInt(ContainerMixEntry.toItemId);
        }
        bs.writeUnsignedVarInt(this->mMaterialReducerEntries.size());
        for (auto& MaterialReducerEntry : mMaterialReducerEntries) {
            bs.writeVarInt(MaterialReducerEntry.fromItemKey);
            bs.writeUnsignedVarInt(MaterialReducerEntry.toItemIdsAndCounts.size());
            for (auto& ita : MaterialReducerEntry.toItemIdsAndCounts) {
                bs.writeVarInt(ita.itemId);
                bs.writeVarInt(ita.itemCount);
            }
        }
        bs.writeBool(this->mClearRecipes);
    } else {
        origin(bs);
    }
}