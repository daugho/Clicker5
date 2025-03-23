#include "Framework.h"

Shop::Shop(int hermitID, OreInventory* inventory, GoldDisplay* goldDisplay)
    : Quad(L"Resources/Textures/UI/Shop1.png"), hermitID(hermitID) , inventory(inventory) , goldDisplay(goldDisplay)
{
    wstring texturePath;

    switch (hermitID) {
    case 1:
        texturePath = L"Resources/Textures/UI/Shop1.png";
        CreateSlots();
        break;
    case 2:
        texturePath = L"Resources/Textures/UI/Shop2.png";
        CreateSlots2();
        break;
    case 3:
        texturePath = L"Resources/Textures/UI/Shop3.png";
        CreateSlots3();
        break;
    default:
        texturePath = L"Resources/Textures/UI/DefaultShop.png";
        CreateSlots();
        break;
    }

    GetMaterial()->SetDiffuseMap(texturePath);
    localPosition = CENTER;
    UpdateWorld();
    isActive = false;
}

Shop::~Shop()
{
    for (ShopSlot* slot : iconSlots) {
        delete slot;
    }
    for (ShopSlot* slot : descSlots) {
        delete slot;
    }
    for (ShopSlot* slot : buySlots) {
        delete slot;
    }
    for (ShopSlot* slot : sellSlots) {
        delete slot;
    }
}

void Shop::Update()
{
    Quad::Update();
    UpdateWorld();
    for (ShopSlot* slot : iconSlots) {
        slot->Update();
    }
    for (ShopSlot* slot : descSlots) {
        slot->Update();
    }
    for (ShopSlot* slot : buySlots) {
        slot->Update();
    }
    for (ShopSlot* slot : sellSlots) {
        slot->Update();
    }
}

void Shop::Render()
{
    //if (!isActive) return;

    Quad::Render();

    for (ShopSlot* slot : iconSlots) {
        slot->Render();
    }
    for (ShopSlot* slot : descSlots) {
        slot->Render();
    }
    for (ShopSlot* slot : buySlots) {
        slot->Render();
    }
    for (ShopSlot* slot : sellSlots) {
        slot->Render();
    }
}

void Shop::Edit()
{
    
    for (ShopSlot* slot : iconSlots) {
        slot->Edit();
    }
}

void Shop::CreateSlots() {

    vector<ShopData>& items = ShopManager::Get()->GetShopTable(hermitID);

    float xinterval = 15.0f;
    float yinterval = 17.0f;
    int numSlots = 3;

    ShopSlot* slot = new ShopSlot();
    slot->SetTag("Ore_ShopSlot");
    slot->SetParent(this);
    slot->SetItem(items[0],0);
    slot->Load();
    iconSlots.push_back(slot);

    ShopSlot* sellButton = new ShopSlot();
    sellButton->SetTag("SellAllButton");
    sellButton->SetParent(this);
    sellButton->Load();
    sellButton->UpdateWorld();
    sellButton->SetEvent([=]() 
        {
        int totalGold = 0;

        auto& slots = inventory->GetSlots(); // inventory는 Shop에 전달된 OreInventory*

        for (auto& slot : slots)
        {
            if (!slot->IsOccupied()) continue;

            DropData item = slot->GetItem();
            int count = slot->GetCount();
            int price = item.price;

            totalGold += price * count;
            inventory->RemoveItemCount(count);
            slot->Clear(); // 판매 후 비우기
        }

        inventory->AddGold(totalGold); // gold 변수 만들어서 증가시키기
        goldDisplay->SetGold(inventory->GetGold());
        OutputDebugStringA(("판매된 금액: " + to_string(totalGold) + "\n").c_str());
        });
    sellSlots.push_back(sellButton);

    for (int i = 1; i < numSlots+1; i++) {
        ShopSlot* slot = new ShopSlot();
        float yOffset = (slot->Size().x + yinterval) * i;
    
        Vector3 pos = iconSlots[0]->GetLocalPosition() + Vector3( 0, yOffset, 0);
        slot->SetLocalPosition(pos);
        slot->SetTag("Ore_ShopSlot"+ to_string(i));
        slot->SetParent(this);
        slot->SetItem(items[i],i);
        slot->Load();

        slot->UpdateWorld();
    
        iconSlots.push_back(slot);
    }
    for (int i = 0; i < numSlots + 1; i++) {
        ShopSlot* slot = new ShopSlot();
        float yOffset = (slot->Size().x + yinterval) * i;

        Vector3 pos = iconSlots[0]->GetLocalPosition() + Vector3(100, yOffset, 0);
        slot->SetLocalPosition(pos);
        slot->SetTag("Ore_ShopSlot_descrip" + to_string(i));
        slot->SetParent(this);
        slot->SetDescrip(items[i], i);

        slot->Load();

        slot->UpdateWorld();

        descSlots.push_back(slot);
    }
    for (int i = 0; i < numSlots + 1; i++) {
        ShopSlot* slot = new ShopSlot();
        float yOffset = (slot->Size().x + yinterval) * i;

        Vector3 pos = iconSlots[0]->GetLocalPosition() + Vector3(465, yOffset, 0);
        slot->SetLocalPosition(pos);
        slot->SetTag("Ore_ShopSlot_Bye" + to_string(i));
        slot->SetParent(this);
        slot->SetSlotIndex(i);
        slot->SetBuyEvent(items[i], i);
        slot->Load();
        slot->UpdateWorld();

        buySlots.push_back(slot);
        RegisterBuyEvent(i);
    }

}

void Shop::CreateSlots2()
{
    vector<ShopData>& items = ShopManager::Get()->GetShopTable(hermitID);

    float xinterval = 15.0f;
    float yinterval = 17.0f;
    int numSlots = 3;

    ShopSlot* slot = new ShopSlot();
    slot->SetTag("Ore_ShopSlot");
    slot->SetParent(this);
    slot->SetItem(items[0], 0);
    slot->Load();
    iconSlots.push_back(slot);

    ShopSlot* sellButton = new ShopSlot();
    sellButton->SetTag("SellAllButton");
    sellButton->SetParent(this);
    sellButton->Load();
    sellButton->UpdateWorld();
    sellButton->SetEvent([=]()
        {
            int totalGold = 0;

            auto& slots = inventory->GetSlots(); // inventory는 Shop에 전달된 OreInventory*

            for (auto& slot : slots)
            {
                if (!slot->IsOccupied()) continue;

                DropData item = slot->GetItem();
                int count = slot->GetCount();
                int price = item.price;

                totalGold += price * count;
                slot->Clear(); // 판매 후 비우기
            }

            inventory->AddGold(totalGold); // gold 변수 만들어서 증가시키기
            goldDisplay->SetGold(inventory->GetGold());
            OutputDebugStringA(("판매된 금액: " + to_string(totalGold) + "\n").c_str());
        });
    sellSlots.push_back(sellButton);
}

void Shop::CreateSlots3()
{
}

void Shop::UpgradePlayer(int index)
{
    Player* player = ClickerMapManager::Get()->GetPlayer();
    if (!player) return;

    switch (index)
    {
    case 0:
        player->ApplyShopDamageBoost();
        OutputDebugStringA("스킬: 채굴 데미지 증가\n");
        break;
    case 1:
        player->ApplyShopSpeedBoost();
        OutputDebugStringA("스킬: 채굴 속도 증가\n");
        break;
    case 2:
        // 이동속도 증가 함수 필요 시 player->ApplyMoveSpeedBoost();
        OutputDebugStringA("스킬: 이동속도 증가\n");
        break;
    default:
        OutputDebugStringA("기타 스킬 구매\n");
        break;
    }
}

void Shop::RegisterBuyEvent(int index)
{
    if (index < 0 || index >= buySlots.size())
        return;

    ShopSlot* slot = buySlots[index];

    slot->SetEvent([=]() {
        ShopData item = slot->GetItem();
        int price = item.price;

        if (inventory->GetGold() < price) {
            OutputDebugStringA("골드 부족!\n");
            return;
        }

        //if (!inventory->AddItem(item, 1)) {
        //    OutputDebugStringA("인벤토리 공간 부족!\n");
        //    return;
        //}
        //나중에 인벤토리가 가득차면 png파일로 알림할것.
        inventory->AddGold(-price);
        goldDisplay->SetGold(inventory->GetGold());

        // 아이콘 및 설명 처리
        if (index < iconSlots.size())
            iconSlots[index]->GetImage()->GetMaterial()->SetDiffuseMap(L"Resources/Textures/UI/ShopUI/soldout.png");

        if (index < descSlots.size())
            descSlots[index]->GetImage()->SetActive(false);

        slot->SetEnabled(false);

        UpgradePlayer(index);
    });
}
