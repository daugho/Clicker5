#include "Framework.h"

int Shop::hasMiner = 0;

Shop::Shop(int hermitID, OreInventory* inventory, GoldDisplay* goldDisplay, Player* player)
    : Quad(L"Resources/Textures/UI/Shop1.png"), hermitID(hermitID) , inventory(inventory) , goldDisplay(goldDisplay), player(player)
{

    GetMaterial()->SetDiffuseMap(texturePath);
    localPosition = CENTER;
    UpdateWorld();
    isActive = false;

    popup = new ItemPopup();
    popup->SetParent(this);
    popup->SetLocalPosition(Vector3(200, 0, 0));
    popup->Load();
    ChangeShop(hermitID);

    notice = new ItemPopup();
	notice->SetParent(this);
	notice->SetLocalPosition(Vector3(200, 0, 0));
	notice->Load();
    Audio::Get()->Add("rein", "Resources/Sounds/rein.mp3");
    Audio::Get()->Add("fail", "Resources/Sounds/fail.mp3");
    Audio::Get()->Add("sell", "Resources/Sounds/sell.mp3");
}

Shop::~Shop()
{
    for (auto r : rateDisplays) delete r;
    rateDisplays.clear();
    ClearSlots();
    delete popup;
    delete notice;
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
	notice->Update();   
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
	notice->Render();
}

void Shop::Edit()
{    
    for (ShopSlot* slot : descSlots) {
        slot->Edit();
    }
    //for (ShopSlot* slot : descSlots) {
    //    slot->Edit();
    //}
    for (ShopSlot* slot : buySlots) {
        slot->Edit();
    }
    //popup->Edit();
    //for (RateDisplay* slot : rateDisplays)
    //    slot->Edit();
}

void Shop::BuyKey(int index)
{
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
    slot->SetItem2(items[0], 0);
    slot->Load();
    iconSlots.push_back(slot);

    for (int i = 1; i < 6; i++) {
        ShopSlot* slot = new ShopSlot();
        float yOffset = (slot->Size().x + yinterval) * i;

        Vector3 pos = iconSlots[0]->GetLocalPosition() + Vector3(0, yOffset, 0);
        slot->SetLocalPosition(pos);
        slot->SetTag("Ore_Shop2Slot" + to_string(i));
        slot->SetParent(this);
        slot->SetItem2(items[i], i);
        slot->Load();

        slot->UpdateWorld();

        iconSlots.push_back(slot);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////
    for (int i = 0; i < 6; i++) {
        ShopSlot* slot = new ShopSlot();
        float yOffset = (slot->Size().x + yinterval) * i;

        Vector3 pos = iconSlots[0]->GetLocalPosition() + Vector3(100, yOffset, 0);
        slot->SetLocalPosition(pos);
        slot->SetTag("Ore_ShopSlot2_descrip" + to_string(i));
        slot->SetParent(this);
        slot->SetDescrip2(items[i], i);
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
            Audio::Get()->Play("sell");

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
        int price = items[i].price;
        if (itemID == 6)
        {
            RegisterBuyEvent(itemID); // index 그대로 넘깁니다
        }
        else
        {
            slot->SetEvent([=]()
                {
                    if (inventory->GetGold() < price) {
                        OutputDebugStringA("골드 부족!\n");
                        return;
                    }

                    inventory->AddGold(-price);
                    goldDisplay->SetGold(inventory->GetGold());

                    TryUpgradeItem(itemID);
                    UpgradePlayer(itemID);
                });
        }
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
    case 1:
		player->ApplyShopSpeedBoost();
        OutputDebugStringA("스킬: 채굴 쿨타임 감소\n");
        break;
    case 2:
        for (BoxInventory* box : Boxmanager::Get()->GetBoxes())
        {
            box->IncreaseCapacity();
        }
        OutputDebugStringA("스킬: 박스 보관량 증가\n");
        break;
    case 3:
        inventory->IncreaseCapacity();
        OutputDebugStringA("스킬: 최대 광물 소지량\n");
        break;
    case 4:
        player->ApplyShopDamageBoost();
        OutputDebugStringA("스킬: 플레이어 근력\n");
        break;
    case 5:
        player->Shop2buypick();
        OutputDebugStringA("스킬: 곡괭이 강화\n");
        break;

    }
}

void Shop::RegisterBuyEvent(int index)
{
    if (index < 0 || index >= buySlots.size())
        return;

    ShopSlot* slot = buySlots[index];
    ShopData item = slot->GetItem();
    int price = item.price;

    switch (hermitID)
    {
    case 1:
    {
        if (ShopPurchaseManager::Get()->IsPurchased(hermitID, index)) {
            slot->SetEnabled(false);
            if (index < iconSlots.size())
                iconSlots[index]->GetImage()->GetMaterial()->SetDiffuseMap(L"Resources/Textures/UI/ShopUI/soldout.png");
            if (index < descSlots.size())
                descSlots[index]->GetImage()->SetActive(false);
            return;
        }

        slot->SetEvent([=]() {
            if (inventory->GetGold() < price) {
                OutputDebugStringA("골드 부족!\n");
                return;
            }

            inventory->AddGold(-price);
            goldDisplay->SetGold(inventory->GetGold());
            ShopPurchaseManager::Get()->SetPurchased(hermitID, index);
            isSoldMap[index] = true;

            if (index < iconSlots.size())
                iconSlots[index]->GetImage()->GetMaterial()->SetDiffuseMap(L"Resources/Textures/UI/ShopUI/soldout.png");
            if (index < descSlots.size())
                descSlots[index]->GetImage()->SetActive(false);
            switch (index)
            {
            case 0: player->Escapekey = 1; break;
            case 1: player->Goldkey = 1; break;
            case 2: player->Silverkey = 1; break;
            case 3: player->Bronzekey = 1; break;
            }
            slot->SetEnabled(false);
            });
        break;
    }
    case 2:
    {
        int itemID = index + 1;

        // ? 이미 구매했으면 비활성화
        if (ShopPurchaseManager::Get()->IsPurchased(hermitID, index))
        {
            slot->SetEnabled(false);
            if (index < iconSlots.size())
                iconSlots[index]->GetImage()->GetMaterial()->SetDiffuseMap(L"Resources/Textures/UI/ShopUI/soldout.png");
            if (index < descSlots.size())
                descSlots[index]->GetImage()->SetActive(false);
            return;
        }

        slot->SetEvent([=]() {
            if (inventory->GetGold() < price) {
                OutputDebugStringA("골드 부족!\n");
                return;
            }

            inventory->AddGold(-price);
            goldDisplay->SetGold(inventory->GetGold());

            TryUpgradeItem(itemID);

            if (index == buySlots.size() - 1)
            {
                Shop::hasMiner = 1;
				notice->Play(L"Resources/Textures/UI/notice.png");

                ShopPurchaseManager::Get()->SetPurchased(hermitID, index);

                slot->SetEnabled(false);
                if (index < iconSlots.size())
                    iconSlots[index]->GetImage()->GetMaterial()->SetDiffuseMap(L"Resources/Textures/UI/ShopUI/soldout.png");
                if (index < descSlots.size())
                    descSlots[index]->GetImage()->SetActive(false);
			}
			});
        break;
    }
    }
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
    // 실제 효과 적용은 여기서 필요에 따라 Player에게 전달
    // 예시:
    // ClickerMapManager::Get()->GetPlayer()->ApplyUpgrade(itemID, data.value);
}

void Shop::ShowUpgradeResultImage(bool success)
{
    if (!popup) return;
    if (success)
    {
        popup->Play(L"Resources/Textures/UI/ShopUI/ok.png");
        Audio::Get()->Play("rein");
    }
    else
    {
        popup->Play(L"Resources/Textures/UI/ShopUI/back.png");
        Audio::Get()->Play("fail");

    }
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
        //CreateSlots3();
        break;
    default:
        texturePath = L"Resources/Textures/UI/DefaultShop.png";
        CreateSlots();
        break;
    }

    GetMaterial()->SetDiffuseMap(texturePath);

    if (hermitID == 1)
    {
        for (int i = 0; i < buySlots.size(); i++) {
            RegisterBuyEvent(i);
        }
    }

    // --- ?? 추가된 부분: 강화 상태 UI 다시 반영 ---
    if (hermitID == 2)
    {
        unordered_map<int, vector<ShopItemLevelData>>& levelDataMap = ShopManager::Get()->GetShop2ItemLevels();

        for (int itemID = 1; itemID <= 5; ++itemID) // 1~5번은 강화 슬롯
        {
            int currentLevel = GetItemLevel(itemID);
            if (currentLevel == 0 || levelDataMap.count(itemID) == 0)
                continue;

            auto& levelList = levelDataMap[itemID];
            int iconLevelIndex = min(currentLevel - 1, (int)levelList.size() - 1);
            wstring newIcon = levelList[iconLevelIndex].iconPath;
            int slotIndex = itemID - 1;

            if (slotIndex < iconSlots.size())
                iconSlots[slotIndex]->GetImage()->GetMaterial()->SetDiffuseMap(newIcon);
            if (slotIndex < levelSlots.size())
                levelSlots[slotIndex]->SetLevel(levelList[iconLevelIndex], slotIndex);
            if (slotIndex < rateDisplays.size())
                rateDisplays[slotIndex]->SetRate(levelList[iconLevelIndex].rate);
        }

        // ?? 마지막 슬롯(index == 5, itemID == 6)는 구매 여부만 판단하여 처리
        int lastIndex = 5;
        if (ShopPurchaseManager::Get()->IsPurchased(hermitID, lastIndex))
        {
            if (lastIndex < buySlots.size())
                buySlots[lastIndex]->SetEnabled(false);
            if (lastIndex < iconSlots.size())
                iconSlots[lastIndex]->GetImage()->GetMaterial()->SetDiffuseMap(L"Resources/Textures/UI/ShopUI/soldout.png");
            if (lastIndex < descSlots.size())
                descSlots[lastIndex]->GetImage()->SetActive(false);
        }

        // RegisterBuyEvent는 index를 기준으로 동작하므로 index == 5 전달
        RegisterBuyEvent(5);
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