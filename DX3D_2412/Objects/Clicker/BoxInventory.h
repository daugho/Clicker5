#pragma once

class BoxInventory : public Cube
{
public:
    BoxInventory(Vector3 position);
    ~BoxInventory();

    void Update();
    void Render();
    void Edit();
    bool AddItem(const DropData& item, int count);
    bool IsFull() const;
    int GetTotalItemCount() const;

private:
    void CreateSlots();
private:
    Model* model;
    //vector<BoxSlot*> slots;
    const int MAX_CAPACITY = 20;
    int currentCapacity = 0;
};