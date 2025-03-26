#pragma once

class ShopSlot;
class GoldDisplay;

class Shop : public Quad
{
public:
    Shop(int hermitID, OreInventory* inventory, GoldDisplay* goldDisplay);
    ~Shop();

    void Update();
    void Render();
    void Edit();
    //shop1
    void CreateSlots();
    void UpgradePlayer(int index);
    void RegisterBuyEvent(int index);
    //shop2
    void CreateSlots2();
    void TryUpgradeItem(int itemID);
    void ShowUpgradeResultImage(bool success);

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
};

