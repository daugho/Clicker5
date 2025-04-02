#pragma once
#include "ShopSlot.h"
#include "HermitData.h"

class ShopSlot : public Button {
public:
    ShopSlot();
    ~ShopSlot();

    void Update();
    void Render();
    void SetItem(const ShopData& item, int index);
    void SetDescrip(const ShopData& item, int index);
    void SetBuyEvent(const ShopData& item, int index);
    void SetBuyEvent2(const ShopData& item, int index);
    void SetLevel(const ShopItemLevelData& level, int index);
    void SetSellbutton(const function<void()>& callback);
    void OnClick();
    //void Edit();


    void SetSlotIndex(int index) { slotIndex = index; }
    int GetSlotIndex() const { return slotIndex; }
    ShopData GetItem() const { return item; }
    Quad* GetImage() const { return image; }
    Transform* GetParentTransform() const { return parent; }
private:
    ShopData item;
    ShopItemLevelData reinforce;
    bool occupied = false;
    vector<ShopSlot*> shopslots;
    ShopSlotType slotType;
    Quad* levelImage = nullptr;
    int slotIndex = -1;
    Transform* parent;
};
