#pragma once

class ShopSlot;
class GoldDisplay;
class RateDisplay;
class ClickerPlayScene;
class Shop : public Quad
{
public:
    static int hasMiner;
    Shop(int hermitID, OreInventory* inventory, GoldDisplay* goldDisplay, Player* player);
    ~Shop();

    void Update();
    void Render();
    void Edit();
    //shop1
    void BuyKey(int index);
    void CreateSlots();
    void RegisterBuyEvent(int index);
    //shop2
    void UpgradePlayer(int index);
    void CreateSlots2();
    void TryUpgradeItem(int itemID);
    void ShowUpgradeResultImage(bool success);
    void InitRateTextures();
    void ChangeShop(int newHermitID);
    void ClearSlots();

    bool IsActive() const { return isActive; }
    vector<ShopSlot*>& GetIconSlots() { return iconSlots; }
    vector<ShopSlot*>& GetDescSlots() { return descSlots; }
    int GetItemLevel(int itemID) const;
    unordered_map<int, int>& GetItemLevels() { return itemLevels; }
private:
    vector<ShopSlot*> iconSlots;
    vector<ShopSlot*> descSlots;
    vector<ShopSlot*> buySlots;
    vector<ShopSlot*> sellSlots;
    vector<ShopSlot*> levelSlots;
    bool isActive = false;
    wstring texturePath;
    int hermitID;
    OreInventory* inventory=nullptr;
    GoldDisplay* goldDisplay = nullptr;
    unordered_map<int, int> itemLevels;
    float resultTimer;
    float resultDuration;
    ItemPopup* popup;
    ItemPopup* notice;
    unordered_map<int, bool> isSoldMap;

    vector<Quad*> rateDigits;
    vector<wstring> numberTextures;
    vector<RateDisplay*> rateDisplays;
    RateDisplay* rateUI;
	Player* player=nullptr;

};

