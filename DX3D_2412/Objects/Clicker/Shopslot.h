#pragma once
#include "ShopSlot.h"
#include "HermitData.h"

class ShopSlot : public Button {
public:
    ShopSlot();
    ~ShopSlot();

    void Update();
    void Render();
    void SetItem(const ShopData& item);
    void OnClick();
    void CreatShop();

private:
    ShopData item;
    bool occupied = false;
    vector<ShopSlot*> shopslots;
    ShopSlotType slotType;
    Quad* iconImage = nullptr;
};
