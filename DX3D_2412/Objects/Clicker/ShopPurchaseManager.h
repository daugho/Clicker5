#pragma once

class ShopPurchaseManager : public Singleton<ShopPurchaseManager>
{
public:
    void SetPurchased(int shopID, int itemIndex);
    bool IsPurchased(int shopID, int itemIndex) const;

private:
    unordered_map<int, unordered_map<int, bool>> purchaseStates; // shopID -> itemIndex -> 구매 여부
};