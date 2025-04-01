#include "Framework.h"

HelperInventory::HelperInventory()
{
}

bool HelperInventory::AddItem(const DropData& item, int count)
{
    if (currentCapacity + count > MAX_CAPACITY)
        return false;

    for (auto& pair : items)
    {
        if (pair.first.id == item.id)
        {
            pair.second += count;
            currentCapacity += count;
            return true;
        }
    }

    items.emplace_back(item, count);
    currentCapacity += count;
    return true;
}

void HelperInventory::Clear()
{
    items.clear();
    currentCapacity = 0;
}

bool HelperInventory::IsFull() const
{
    return currentCapacity >= MAX_CAPACITY;
}

int HelperInventory::GetTotalItemCount() const
{
    return currentCapacity;
}