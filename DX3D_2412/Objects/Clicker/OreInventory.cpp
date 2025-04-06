#include "Framework.h"

OreInventory::OreInventory() : Quad(L"Resources/Textures/UI/Bag.png")
{
    SetTexture(L"Resources/Textures/UI/Bag.png");
    localPosition = CENTER;
    UpdateWorld();
    CreatSlot();
    //light = Environment::Get()->GetLight(0);
    //light->type = 2;
    //light->color = { 1, 1, 1, 1 };
}

OreInventory::~OreInventory()
{
    for (OreSlot* slot : slots) {
        delete slot;
    }
}

void OreInventory::Update()
{
	Quad::Update();
    UpdateWorld();
    for (OreSlot* slot : slots) {
        slot->Update();
    }
}

void OreInventory::Render()
{
    //if (!isActive) return;
    Quad::Render();

    for (OreSlot* slot : slots) {
        slot->Render();
    }
}

void OreInventory::Edit()
{
    for (OreSlot* slot : slots) {
        slot->Edit();
    }
}

void OreInventory::CreatSlot()
{
    OreSlot* slot = new OreSlot();
    slot->SetTag("Ore_InventorySlot");
    slot->SetParent(this);
    slot->SetInventory(this);
    slot->Load();
    slots.push_back(slot);
    
    float interval = 7.0f;
    float  Hinterval = 5.0f;
    for (int i = 1; i < 5; i++)
    {
        OreSlot* slot = new OreSlot();
        float xOffset = (slot->Size().x + interval) * i;

        Vector3 pos = slots[0]->GetLocalPosition() + Vector3(xOffset, 0, 0);
        slot->SetLocalPosition(pos);
        slot->SetParent(this);
        slot->SetInventory(this);
        slot->UpdateWorld();
        //slot->Load();
        slots.push_back(slot);
    }

    for (int j = 1; j < 3; j++)
    {
        for (int i = 0; i < 5; i++)
        {
            OreSlot* slot = new OreSlot();
            float xOffset = (slot->Size().x + interval) * i;
            float yOffset = (slot->Size().y + Hinterval) * -j;

            Vector3 pos = slots[0]->GetLocalPosition() + Vector3(xOffset, yOffset, 0);
            slot->SetLocalPosition(pos);
            slot->SetParent(this);
            slot->SetInventory(this);
            slot->UpdateWorld();
            //slot->Load();

            slots.push_back(slot);
        }
    }
}

void OreInventory::IncreaseCapacity()
{
	MAX_CAPACITY += 10;
    OutputDebugStringA(("IncreaseCapacity 호출됨 → MAX_CAPACITY = " + to_string(MAX_CAPACITY) + "\n").c_str());
}

bool OreInventory::AddItem(const DropData& item, int count)
{
    wstring msg1 = L"[OreInventory] this = " + to_wstring((UINT64)this) + L"\n";
    OutputDebugStringA(("MAX_CAPACITY = " + to_string(MAX_CAPACITY) + ", currentCapacity = " + to_string(currentCapacity) + "\n").c_str());

    OutputDebugString(msg1.c_str());
    int availableSpace = MAX_CAPACITY - currentCapacity;
    if (availableSpace <= 0)
        return false; // 아예 여유 없음

    int addCount = min(availableSpace, count); // 실제로 추가 가능한 개수

    // 이미 존재하는 아이템에 추가
    for (auto& slot : slots) {
        if (slot->IsOccupied() && slot->GetItem().id == item.id) {
            int newCount = slot->GetCount() + addCount;
            slot->SetItem(item, newCount);
            currentCapacity += addCount;

            ClickerUIManager::Get()->GetItemPopup()->Play(item.rarePath);
            return true;
        }
    }

    // 새로운 슬롯에 추가
    for (auto& slot : slots) {
        if (!slot->IsOccupied()) {
            slot->SetItem(item, addCount);
            currentCapacity += addCount;

            ClickerUIManager::Get()->GetItemPopup()->Play(item.rarePath);
            return true;
        }
    }
    return false;
}

int OreInventory::GetTotalItemCount() const
{
    int totalCount = 0;
    for (const auto& slot : slots) {
        if (slot->IsOccupied())
            totalCount += slot->GetCount();
    }
    return totalCount;
}

void OreInventory::RemoveItemCount(int count)
{
    currentCapacity -= count;
    currentCapacity = max(0, currentCapacity);
}

