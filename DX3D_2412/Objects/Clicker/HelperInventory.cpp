#include "Framework.h"

HelperInventory::HelperInventory()
{
    
}

bool HelperInventory::AddItem(const DropData& item, int count)
{
    wstring msg1 = L"[HelperInventory] this = " + to_wstring((UINT64)this) + L"\n";
    OutputDebugString(msg1.c_str());
    if (currentCapacity + count > MAX_CAPACITY)
        return false;

    for (auto& pair : items)
    {
        if (pair.first.id == item.id)
        {
            pair.second += count;
            currentCapacity += count;
            wstring msg = L"[HelperInventory] ���� ������ �߰���: ID = " + to_wstring(item.id) +
                L", ���� = " + to_wstring(count) +
                L", �� ���� = " + to_wstring(pair.second) + L"\n";
            OutputDebugString(msg.c_str());
            return true;
        }
    }

    items.emplace_back(item, count);
    currentCapacity += count;
    wstring msg = L"[HelperInventory] �� ������ �߰���: ID = " + to_wstring(item.id) +
        L", ���� = " + to_wstring(count) + L"\n";
    OutputDebugString(msg.c_str());
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

