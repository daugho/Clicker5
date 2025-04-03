#pragma once

class BoxInventory : public Cube
{
public:
    BoxInventory(Vector3 position);
    ~BoxInventory();

    void Update();
    void Render();
    void Edit();
    bool IsFull() const;
    int GetTotalItemCount() const;
    bool AddItem(const DropData& item, int count);
    void TakeItem(OreInventory* inventory);
    const vector<pair<DropData, int>>& GetItems() const;

private:
    void CreateSlots();
private:
    Model* model;
    //vector<BoxSlot*> slots;
    const int MAX_CAPACITY = 20;
    int currentCapacity = 0;
    vector<pair<DropData, int>> items; // 드랍 아이템 + 개수
};