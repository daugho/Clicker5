#pragma once
class OreSlot;
class OreInventory : public Quad
{
public:
    OreInventory();
    ~OreInventory();

    void Update();
    void Render();
    void Edit();
    void CreatSlot();
    void IncreaseCapacity();
    bool AddItem(const DropData& item, int count);
    void RemoveItemCount(int count);
    
    void AddGold(int value) { gold += value; }
    int GetGold() const { return gold; }
    int GetMaxCapacity() const { return MAX_CAPACITY; }
    bool IsActive() const { return isActive; }
    int GetTotalItemCount() const;
    vector<OreSlot*>& GetSlots() { return slots; }

private:

    int currentCapacity = 0;
    int MAX_CAPACITY = 10;
    const int START_SLOTS = 5;

    const int GRID_ROWS = 3;
    const int GRID_COLS = 5;
    const int TOTAL_SLOTS = GRID_ROWS * GRID_COLS;

    vector<OreSlot*> slots;

    bool isActive = false;
    LightBuffer::Light* light;
    int gold =100000000 ;
private:
    struct InventorySlot 
    {
        DropData item;// 아이템 데이터
        int count;
        bool occupied = false;
    };
};