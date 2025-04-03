#include "Framework.h"

BoxInventory::BoxInventory(Vector3 position)
{
    model = new Model("woodbox");
    model->SetParent(this);
    model->SetLocalPosition(position);
    model->Load();
    collider = new BoxCollider(Vector3(2, 1, 1));
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
    //model->Edit();
    //collider->Edit();
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
    OutputDebugString(L"[BoxInventory] AddItem ȣ���!\n");

    wstring log = L"�߰��� ������ ID = " + to_wstring(item.id) +
        L", ���� = " + to_wstring(count) + L"\n";
    OutputDebugString(log.c_str());
    // �뷮 �ʰ� üũ
    if (currentCapacity + count > MAX_CAPACITY)
        return false;

    // �̹� �����ϴ� �������̸� count�� ����
    for (auto& pair : items)
    {
        if (pair.first.id == item.id)
        {
            pair.second += count;
            currentCapacity += count;
            return true;
        }
    }

    // ���ο� ������ �߰�
    items.emplace_back(item, count);
    currentCapacity += count;
    return true;
}

void BoxInventory::TakeItem(OreInventory* inventory)
{
    for (auto it = items.begin(); it != items.end(); )
    {
        DropData& item = it->first;
        int count = it->second;

        bool added = inventory->AddItem(item, count);
        if (added)
        {
            currentCapacity -= count;
            it = items.erase(it);  // �������� �ű� �� ����
        }
        else
        {
            ++it; // �ű��� �������� �Ѿ
        }
    }
}

const vector<pair<DropData, int>>& BoxInventory::GetItems() const
{
    return items;
}

void BoxInventory::CreateSlots()
{
}
