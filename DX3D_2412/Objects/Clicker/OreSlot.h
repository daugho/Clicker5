#pragma once
#include "HermitData.h"

class OreSlot : public Button 
{
public:
    OreSlot();
    ~OreSlot();

    void Update();
    void SetItem(const DropData& item, int count);
    void Render();
    void ShowTooltip();
    void Clear();
    void Edit();

    void SetData(DropData data,int index);
    bool IsOccupied() const { return occupied; }
    const DropData& GetItem() const { return item; }
    void SetInventory(OreInventory* inventory) { this->inventory = inventory; }
    int GetCount() const { return count; }//0329
    void UpdateData();

private:
    DropData item;
    int count = 0;
    bool occupied = false;
    int index = 0;
    OreInventory* inventory = nullptr;
};