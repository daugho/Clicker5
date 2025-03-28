#include "Framework.h"

void ShopPurchaseManager::SetPurchased(int shopID, int itemIndex)
{
	purchaseStates[shopID][itemIndex] = true;
}

bool ShopPurchaseManager::IsPurchased(int shopID, int itemIndex) const
{
    auto shopIt = purchaseStates.find(shopID);
    if (shopIt != purchaseStates.end())
    {
        auto itemIt = shopIt->second.find(itemIndex);
        return itemIt != shopIt->second.end() && itemIt->second;
    }
    return false;
}
