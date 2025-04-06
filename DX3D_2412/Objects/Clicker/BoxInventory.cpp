#include "Framework.h"

BoxInventory::BoxInventory(Vector3 position)
{
    model = new Model("woodbox");
    model->SetTag("BoxModel");
    model->SetParent(this);
    model->SetLocalPosition(position);
    model->Load();

    collider->SetTag("BoxCollider");
    collider->SetLocalScale(Vector3(2, 1, 1));
    OutputDebugString((L"BoxCollider호출됨: "));
    collider->SetLocalPosition(position.x, position.y+0.5, position.z);
    collider->SetParent(this);
    collider->Load();
}

BoxInventory::~BoxInventory()
{
    delete model;
    delete collider;
}

void BoxInventory::Update()
{
    model->UpdateWorld();
    collider->Update();
}

void BoxInventory::Render()
{
    model->Render();
    collider->Render();
}

void BoxInventory::Edit()
{
    model->Edit();
    collider->Edit();
}

bool BoxInventory::IsFull() const
{
    return false;
}

int BoxInventory::GetTotalItemCount() const
{
    return 0;
}

bool BoxInventory::AddItem(const DropData& item, int count)
{
    OutputDebugString(L"[BoxInventory] AddItem 호출됨!\n");

    wstring log = L"추가된 아이템 ID = " + to_wstring(item.id) +
        L", 개수 = " + to_wstring(count) + L"\n";
    OutputDebugString(log.c_str());
    // 용량 초과 체크
    if (currentCapacity + count > MAX_CAPACITY)
        return false;

    // 이미 존재하는 아이템이면 count만 증가
    for (auto& pair : items)
    {
        if (pair.first.id == item.id)
        {
            pair.second += count;
            currentCapacity += count;
            return true;
        }
    }

    // 새로운 아이템 추가
    items.emplace_back(item, count);
    currentCapacity += count;
    return true;
}

void BoxInventory::TakeItem(OreInventory* inventory)
{
    for (auto it = items.begin(); it != items.end(); )
    {
        DropData& item = it->first;
        int boxItemCount = it->second;

        int oreFreeSpace = inventory->GetMaxCapacity() - inventory->GetTotalItemCount(); // 여유 공간 계산
        if (oreFreeSpace <= 0)
            break; // 더 이상 꺼낼 수 없음

        int transferCount = min(boxItemCount, oreFreeSpace); // 옮길 수 있는 최대치

        bool added = inventory->AddItem(item, transferCount);
        if (added)
        {
            RemoveItemCount(transferCount);
            it->second -= transferCount;

            if (it->second <= 0)
                it = items.erase(it);
            else
                ++it;
        }
        else
        {
            ++it;
        }
    }
    //for (auto it = items.begin(); it != items.end(); )
    //{
    //    DropData& item = it->first;
    //    int count = it->second;
    //
    //    bool added = inventory->AddItem(item, count);
    //    if (added)
    //    {
    //        currentCapacity -= count;
    //        it = items.erase(it);  // 아이템을 옮긴 후 제거
    //    }
    //    else
    //    {
    //        ++it; // 옮기지 못했으면 넘어감
    //    }
    //}
}

void BoxInventory::RemoveItemCount(int count)
{
    currentCapacity -= count;
    currentCapacity = max(0, currentCapacity);
}

const vector<pair<DropData, int>>& BoxInventory::GetItems() const
{
    return items;
}

void BoxInventory::IncreaseCapacity()
{
    MAX_CAPACITY += 20;
}

void BoxInventory::CreateSlots()
{
}
