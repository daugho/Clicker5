#pragma once

class HelperInventory
{
public:
    HelperInventory();
    ~HelperInventory() = default;

    bool AddItem(const DropData& item, int count);
    void Clear();

    bool IsFull() const;
    int GetTotalItemCount() const { return currentCapacity; }

    const std::vector<std::pair<DropData, int>>& GetItems() const { return items; }

private:
    const int MAX_CAPACITY = 5;
    int currentCapacity = 0;

    std::vector<std::pair<DropData, int>> items;
};
