#pragma once

class ShopSlot;
class GoldDisplay;

class Shop : public Quad {
public:
    Shop(int hermitID, OreInventory* inventory, GoldDisplay* goldDisplay);
    ~Shop();

    void Update();
    void Render();
    void Edit();
    void CreateSlots();
    void CreateSlots2();
    void CreateSlots3();
    void UpgradePlayer(int index);

    void RegisterBuyEvent(int index);

    void TryUpgradeItem(int itemID);

    bool IsActive() const { return isActive; }
    vector<ShopSlot*>& GetIconSlots() { return iconSlots; }
    vector<ShopSlot*>& GetDescSlots() { return descSlots; }

private:
    vector<ShopSlot*> iconSlots;
    vector<ShopSlot*> descSlots;
    vector<ShopSlot*> buySlots;
    vector<ShopSlot*> sellSlots;
    bool isActive = false;
    wstring texturePath;
    int hermitID;
    OreInventory* inventory=nullptr;
    GoldDisplay* goldDisplay = nullptr;
    unordered_map<int, int> itemLevels;
};

