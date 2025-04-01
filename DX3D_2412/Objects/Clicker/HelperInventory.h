#pragma once
#include "DropData.h" // DropData 정의가 포함된 헤더를 포함하세요

class HelperInventory
{
public:
    HelperInventory();
    ~HelperInventory() = default;

    bool AddItem(const DropData& item, int count);
    void Clear();

    bool IsFull() const;
    int GetTotalItemCount() const;

    const std::vector<std::pair<DropData, int>>& GetItems() const { return items; }

private:
    const int MAX_CAPACITY = 10;
    int currentCapacity = 0;

    std::vector<std::pair<DropData, int>> items;
};
