#include "Framework.h"

Shop::Shop(int hermitID, OreInventory* inventory, GoldDisplay* goldDisplay)
    : Quad(L"Resources/Textures/UI/Shop1.png"), hermitID(hermitID) , inventory(inventory) , goldDisplay(goldDisplay)
{
    //wstring texturePath;
    //
    //switch (hermitID) {
    //case 1:
    //    texturePath = L"Resources/Textures/UI/Shop1.png";
    //    CreateSlots();
    //    break;
    //case 2:
    //    texturePath = L"Resources/Textures/UI/Shop2.png";
    //    CreateSlots2();
    //    break;
    //case 3:
    //    texturePath = L"Resources/Textures/UI/Shop3.png";
    //    break;
    //default:
    //    texturePath = L"Resources/Textures/UI/DefaultShop.png";
    //    CreateSlots();
    //    break;
    //}

    GetMaterial()->SetDiffuseMap(texturePath);
    localPosition = CENTER;
    UpdateWorld();
    isActive = false;

    popup = new ItemPopup();
    popup->SetParent(this);
    popup->SetLocalPosition(Vector3(200, 0, 0));
    popup->Load();
    ChangeShop(hermitID);
}

Shop::~Shop()
{
    for (auto r : rateDisplays) delete r;
    rateDisplays.clear();
    ClearSlots();
    delete popup;
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
    for (ShopSlot* slot : levelSlots) {
        slot->Update();
    }
    for (auto rate : rateDisplays)
        rate->Update();
    popup->Update();
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
    for (ShopSlot* slot : levelSlots) {
        slot->Render();
    }
    if (hermitID == 2) {
        for (auto rate : rateDisplays)
            rate->Render();
    }
    popup->Render();
}

void Shop::Edit()
{    
    for (ShopSlot* slot : iconSlots) {
        slot->Edit();
    }
    //popup->Edit();
    //for (RateDisplay* slot : rateDisplays)
    //    slot->Edit();
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
    sellButton->SetSellbutton([=]() 
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
    vector<int> itemIDs = { 1, 2, 3, 4, 5, 6 };//업그레이드 가능한칸은 5칸이지만 총슬롯은 6개이므로 6이 맞다.

    float xinterval = 15.0f;
    float yinterval = 17.0f;
    int numSlots = 3;

    ShopSlot* slot = new ShopSlot();
    slot->SetTag("Ore_Shop2Slot");
    slot->SetParent(this);
    slot->SetItem(items[0], 0);
    slot->Load();
    iconSlots.push_back(slot);

    for (int i = 1; i < 6; i++) {
        ShopSlot* slot = new ShopSlot();
        float yOffset = (slot->Size().x + yinterval) * i;

        Vector3 pos = iconSlots[0]->GetLocalPosition() + Vector3(0, yOffset, 0);
        slot->SetLocalPosition(pos);
        slot->SetTag("Ore_Shop2Slot" + to_string(i));
        slot->SetParent(this);
        slot->SetItem(items[i], i);
        slot->Load();

        slot->UpdateWorld();

        iconSlots.push_back(slot);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////
    for (int i = 0; i < 4 + 1; i++) {
        ShopSlot* slot = new ShopSlot();
        float yOffset = (slot->Size().x + yinterval) * i;

        Vector3 pos = iconSlots[0]->GetLocalPosition() + Vector3(100, yOffset, 0);
        slot->SetLocalPosition(pos);
        slot->SetTag("Ore_ShopSlot2_descrip" + to_string(i));
        slot->SetParent(this);
        slot->SetDescrip(items[i], i);
        slot->Load();

        slot->UpdateWorld();

        descSlots.push_back(slot);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////
    ShopSlot* sellButton = new ShopSlot();
    sellButton->SetTag("SellAllButton2");
    sellButton->SetParent(this);
    sellButton->Load();
    sellButton->UpdateWorld();
    sellButton->SetSellbutton([=]()
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
    /////////////////////////////////////////////////////////////////////////////////////////////
    for (int i = 0; i < 6; i++)
    {
        ShopSlot* slot = new ShopSlot();
        float yOffset = (slot->Size().x + yinterval) * i;

        Vector3 pos = iconSlots[0]->GetLocalPosition() + Vector3(465, yOffset, 0);
        slot->SetLocalPosition(pos);
        slot->SetTag("Ore_Shop2Slot_Bye" + to_string(i));
        slot->SetParent(this);
        slot->SetBuyEvent2(items[i], i);
        int itemID = itemIDs[i];
        slot->SetEvent([=]()
            {
                TryUpgradeItem(itemID);
            });
        slot->SetSlotIndex(i);
        int level = GetItemLevel(itemID);
        slot->Load();
        slot->UpdateWorld();
        buySlots.push_back(slot);
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (int i = 0; i < 6; i++)
    {
        int itemID = itemIDs[i];

        // 레벨 데이터 가져오기
        const auto& levels = ShopManager::Get()->GetShop2ItemLevels();
        if (levels.count(itemID) == 0 || levels.at(itemID).empty())
            continue;

        const ShopItemLevelData& levelData = levels.at(itemID)[0];  // 레벨 1 데이터

        ShopSlot* slot = new ShopSlot();
        float yOffset = (slot->Size().x + yinterval) * i;

        Vector3 pos = iconSlots[0]->GetLocalPosition() + Vector3(0, yOffset, 0); // 위치 조정
        slot->SetLocalPosition(pos);
        slot->SetTag("ForceLevel" + to_string(i));
        slot->SetParent(this);
        slot->SetLevel(levelData, i);
        slot->Load();
        slot->UpdateWorld();

        levelSlots.push_back(slot);

        rateUI = new RateDisplay();
        rateUI->SetPosition(pos + Vector3(200, 0, 0)); // 원하는 위치로 조정
        rateUI->SetSlotID(i);
        rateUI->SetRate(levelData.rate);
        rateUI->Load();
        rateUI->UpdateWorld();
        
        rateDisplays.push_back(rateUI);
        OutputDebugStringA(("rate: " + to_string(levelData.rate) + "\n").c_str());
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
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

    if (hermitID == 2)
    {
        // shop2는 업그레이드 슬롯이므로 별도 처리
        int itemID = index + 1;
        slot->SetEvent([=]() {
            TryUpgradeItem(itemID);
            });
        return;
    }

    if (ShopPurchaseManager::Get()->IsPurchased(hermitID, index)) {
        slot->SetEnabled(false);
        if (index < iconSlots.size())
            iconSlots[index]->GetImage()->GetMaterial()->SetDiffuseMap(L"Resources/Textures/UI/ShopUI/soldout.png");
        if (index < descSlots.size())
            descSlots[index]->GetImage()->SetActive(false);
        return;
    }

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
        ShopPurchaseManager::Get()->SetPurchased(hermitID, index);
        isSoldMap[index] = true;
        // 아이콘 및 설명 처리
        if (index < iconSlots.size())
            iconSlots[index]->GetImage()->GetMaterial()->SetDiffuseMap(L"Resources/Textures/UI/ShopUI/soldout.png");

        if (index < descSlots.size())
            descSlots[index]->GetImage()->SetActive(false);

        slot->SetEnabled(false);

        UpgradePlayer(index);
    });
}

void Shop::TryUpgradeItem(int itemID)
{
    int& currentLevel = itemLevels[itemID];
    if (currentLevel == 0)
        currentLevel = 1;

    unordered_map<int, vector<ShopItemLevelData>>& levels = ShopManager::Get()->GetShop2ItemLevels(); // CSV에서 로드된 데이터
    if (levels.count(itemID) == 0) { return; }


    auto& levelList = levels[itemID];

    // 범위 초과 방지
    if (currentLevel > levelList.size())
    {
        OutputDebugStringA("이미 최대 레벨입니다!\n");
        return;
    }

    // 현재 레벨의 강화 정보
    ShopItemLevelData& data = levelList[currentLevel - 1];

    static std::random_device rd;
    static std::mt19937 generator(rd());
    std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

    // 난수 생성
    float randValue = distribution(generator);
    bool success = randValue <= data.rate;

    if (success)
    {
        currentLevel++;
        OutputDebugStringA(("강화 성공! 현재 레벨: " + to_string(currentLevel) + "\n").c_str());
    }
    else
    {
        int oldLevel = currentLevel;
        currentLevel = max(1, currentLevel - data.down);
        OutputDebugStringA(("강화 실패! 레벨 감소: " + to_string(oldLevel) + " → " + to_string(currentLevel) + "\n").c_str());
    }
    ShowUpgradeResultImage(success);
    // UI 아이콘 변경
    int slotIndex = itemID - 1; // buySlots와 itemID가 1부터 일치한다고 가정
    if (slotIndex >= 0 && slotIndex < iconSlots.size())
    {
        int iconLevelIndex = min(currentLevel - 1, (int)levelList.size() - 1);
        wstring newIcon = levelList[iconLevelIndex].iconPath;
        iconSlots[slotIndex]->GetImage()->GetMaterial()->SetDiffuseMap(newIcon);
    }
    if (slotIndex >= 0 && slotIndex < levelSlots.size())
    {
        int levelIndex = min(currentLevel - 1, (int)levelList.size() - 1);
        levelSlots[slotIndex]->SetLevel(levelList[levelIndex], slotIndex);
        if (slotIndex < rateDisplays.size())
            rateDisplays[slotIndex]->SetRate(levelList[levelIndex].rate);
    }
    switch (itemID) {
    case 1:

        break;
    case 2:

        break;
    case 3:

        break;
    case 4 :

        break;
    case 5:

        break;
    }
    // 실제 효과 적용은 여기서 필요에 따라 Player에게 전달
    // 예시:
    // ClickerMapManager::Get()->GetPlayer()->ApplyUpgrade(itemID, data.value);
}

void Shop::ShowUpgradeResultImage(bool success)
{
    if (!popup) return;
    if (success)
        popup->Play(L"Resources/Textures/UI/ShopUI/ok.png");
    else
        popup->Play(L"Resources/Textures/UI/ShopUI/back.png");
}

void Shop::InitRateTextures()
{
    for (int i = 0; i <= 9; ++i)
        numberTextures.push_back(L"Resources/Textures/UI/" + to_wstring(i) + L".png");

    numberTextures.push_back(L"Resources/Textures/UI/dot.png"); // '.'용
}

void Shop::ChangeShop(int newHermitID)
{
    ClearSlots();

    hermitID = newHermitID;

    // 텍스처 변경 및 슬롯 생성
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
        break;
    default:
        texturePath = L"Resources/Textures/UI/DefaultShop.png";
        CreateSlots();
        break;
    }

    GetMaterial()->SetDiffuseMap(texturePath);
    for (int i = 0; i < buySlots.size(); i++) {
        RegisterBuyEvent(i);
    }
}

void Shop::ClearSlots()
{
    for (ShopSlot* slot : iconSlots) delete slot;
    for (ShopSlot* slot : descSlots) delete slot;
    for (ShopSlot* slot : buySlots) delete slot;
    for (ShopSlot* slot : sellSlots) delete slot;
    for (ShopSlot* slot : levelSlots) delete slot;
    for (RateDisplay* rate : rateDisplays) delete rate;

    iconSlots.clear();
    descSlots.clear();
    buySlots.clear();
    sellSlots.clear();
    levelSlots.clear();
    rateDisplays.clear();
}

int Shop::GetItemLevel(int itemID) const
{
    auto it = itemLevels.find(itemID);
    if (it != itemLevels.end())
        return it->second;

    return 1; // 기본 레벨은 1
}




