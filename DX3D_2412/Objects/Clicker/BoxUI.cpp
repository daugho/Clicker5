#include "Framework.h"
#include "BoxUI.h"

BoxUI::BoxUI() : Quad(L"Resources/Textures/UI/woodbox.png")
{
    localPosition = CENTER;
    UpdateWorld();
    isActive = false;
    CreatSlot();

    insertButton = new BoxSlot();
    insertButton->SetParent(this);
    insertButton->SetTag("Insert");
    insertButton->SetLocalPosition(Vector3(-50, -50, 0));
    insertButton->SetImage(L"Resources/Textures/UI/put.png",0);
    insertButton->Load();
    insertButton->UpdateWorld();
    insertButton->SetEvent([=]()//0329
    {
        OreInventory* inventory = ClickerUIManager::Get()->GetInventory();
        const auto& slots = inventory->GetSlots(); // OreSlot 리스트

        for (OreSlot* slot : slots)
        {
            if (!slot->IsOccupied()) continue;

            const DropData& item = slot->GetItem();
            int count = slot->GetCount();

            if (targetBox->AddItem(item, count)) // BoxInventory에 추가 성공 시
            {
                slot->Clear(); // 슬롯 비우기
            }
        }
        UpdateSlots();
    });
    insertButton->SetActive(false);

    removeButton = new BoxSlot();
    removeButton->SetParent(this);
    removeButton->SetTag("Remove");
    removeButton->SetImage(L"Resources/Textures/UI/remove.png",1);
    removeButton->Load();
    removeButton->UpdateWorld();
    removeButton->SetEvent([=]()
    {
        OutputDebugStringA("빼기 버튼 클릭됨\n");
    });
    removeButton->SetActive(false);

}

BoxUI::~BoxUI()
{
    delete insertButton;
    delete removeButton;
    for (BoxSlot* slot : boxslots) {
        delete slot;
    }
}

void BoxUI::Update()
{
    Quad::Update();
    UpdateWorld();
    insertButton->Update();
    removeButton->Update();
    for (BoxSlot* slot : boxslots) {
        slot->Update();
    }
}

void BoxUI::Render()
{
    if (!isVisible) return;

    Quad::Render();

    if (insertButton) insertButton->Render();
    if (removeButton) removeButton->Render();
    for (BoxSlot* slot : boxslots)
        slot->Render();
}

void BoxUI::Edit()
{
    //insertButton->Edit();
    //removeButton->Edit();
    //for (BoxSlot* slot : boxslots)
    //    slot->Edit();
}

void BoxUI::CreatSlot()
{
    BoxSlot* slot = new BoxSlot();
    slot->SetTag("Box_Slot_0");
    slot->SetParent(this);
    slot->Load();
    boxslots.push_back(slot);
    
    float interval = 7.0f;
    float  Hinterval = 12.5f;
    for (int i = 1; i < 12; i++)
    {
        BoxSlot* slot = new BoxSlot();
        float xOffset = (slot->Size().x + interval) * i;

        Vector3 pos = boxslots[0]->GetLocalPosition() + Vector3(xOffset, 0, 0);
        slot->SetLocalPosition(pos);
        slot->SetParent(this);
        slot->SetTag("Box_slot_" + to_string(i));
        slot->UpdateWorld();
        slot->Load();
        boxslots.push_back(slot);
    }
    for (int j = 1; j < 2; j++)
    {
        for (int i = 0; i < 11; i++)
        {
            BoxSlot* slot = new BoxSlot();
            float xOffset = (slot->Size().x + interval) * i;
            float yOffset = (slot->Size().y + Hinterval) * -j;

            Vector3 pos = boxslots[0]->GetLocalPosition() + Vector3(xOffset, yOffset, 0);
            slot->SetLocalPosition(pos);
            slot->SetParent(this);
            slot->SetTag("Box_slot_" + to_string(j)+"_" + to_string(i));
            slot->Load();
            slot->UpdateWorld();

            boxslots.push_back(slot);
        }
    }
}

void BoxUI::SetVisible(bool visible)
{
    isVisible = visible;
    SetActive(visible); // 전체 UI 자체를 On/Off
    if (insertButton) insertButton->SetActive(visible);
    if (removeButton) removeButton->SetActive(visible);
}

void BoxUI::SetTargetBox(BoxInventory* box)
{
    targetBox = box;
}

void BoxUI::UpdateSlots()
{
    const auto& items = targetBox->GetItems();

    for (int i = 0; i < items.size() && i < boxslots.size(); ++i)
    {
        const DropData& data = items[i].first;
        int count = items[i].second;

        boxslots[i]->SetItem(data, count);
    }
    for (int i = items.size(); i < boxslots.size(); ++i)
    {
        boxslots[i]->ClearItem();
    }
}
