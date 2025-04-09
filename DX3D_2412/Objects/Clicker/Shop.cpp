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

        auto& slots = inventory->GetSlots(); // inventory�� Shop�� ���޵� OreInventory*

        for (auto& slot : slots)
        {
            if (!slot->IsOccupied()) continue;

            DropData item = slot->GetItem();
            int count = slot->GetCount();
            int price = item.price;

            totalGold += price * count;
            inventory->RemoveItemCount(count);
            slot->Clear(); // �Ǹ� �� ����
        }

        inventory->AddGold(totalGold); // gold ���� ���� ������Ű��
        goldDisplay->SetGold(inventory->GetGold());
        OutputDebugStringA(("�Ǹŵ� �ݾ�: " + to_string(totalGold) + "\n").c_str());
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
    vector<int> itemIDs = { 1, 2, 3, 4, 5, 6 };//���׷��̵� ������ĭ�� 5ĭ������ �ѽ����� 6���̹Ƿ� 6�� �´�.

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

            auto& slots = inventory->GetSlots(); // inventory�� Shop�� ���޵� OreInventory*

            for (auto& slot : slots)
            {
                if (!slot->IsOccupied()) continue;

                DropData item = slot->GetItem();
                int count = slot->GetCount();
                int price = item.price;

                totalGold += price * count;
                slot->Clear(); // �Ǹ� �� ����
            }

            inventory->AddGold(totalGold); // gold ���� ���� ������Ű��
            goldDisplay->SetGold(inventory->GetGold());
            OutputDebugStringA(("�Ǹŵ� �ݾ�: " + to_string(totalGold) + "\n").c_str());
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
            RegisterBuyEvent(itemID); // index �״�� �ѱ�ϴ�
        }
        else
        {
            slot->SetEvent([=]()
                {
                    if (inventory->GetGold() < price) {
                        OutputDebugStringA("��� ����!\n");
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

        // ���� ������ ��������
        const auto& levels = ShopManager::Get()->GetShop2ItemLevels();
        if (levels.count(itemID) == 0 || levels.at(itemID).empty())
            continue;

        const ShopItemLevelData& levelData = levels.at(itemID)[0];  // ���� 1 ������

        ShopSlot* slot = new ShopSlot();
        float yOffset = (slot->Size().x + yinterval) * i;

        Vector3 pos = iconSlots[0]->GetLocalPosition() + Vector3(0, yOffset, 0); // ��ġ ����
        slot->SetLocalPosition(pos);
        slot->SetTag("ForceLevel" + to_string(i));
        slot->SetParent(this);
        slot->SetLevel(levelData, i);
        slot->Load();
        slot->UpdateWorld();

        levelSlots.push_back(slot);

        rateUI = new RateDisplay();
        rateUI->SetPosition(pos + Vector3(200, 0, 0)); // ���ϴ� ��ġ�� ����
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
        OutputDebugStringA("��ų: ä�� ��Ÿ�� ����\n");
        break;
    case 2:
        for (BoxInventory* box : Boxmanager::Get()->GetBoxes())
        {
            box->IncreaseCapacity();
        }
        OutputDebugStringA("��ų: �ڽ� ������ ����\n");
        break;
    case 3:
        inventory->IncreaseCapacity();
        OutputDebugStringA("��ų: �ִ� ���� ������\n");
        break;
    case 4:
        player->ApplyShopDamageBoost();
        OutputDebugStringA("��ų: �÷��̾� �ٷ�\n");
        break;
    case 5:
        player->Shop2buypick();
        OutputDebugStringA("��ų: ��� ��ȭ\n");
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
                OutputDebugStringA("��� ����!\n");
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

        // ? �̹� ���������� ��Ȱ��ȭ
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
                OutputDebugStringA("��� ����!\n");
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

    unordered_map<int, vector<ShopItemLevelData>>& levels = ShopManager::Get()->GetShop2ItemLevels(); // CSV���� �ε�� ������
    if (levels.count(itemID) == 0) { return; }


    auto& levelList = levels[itemID];

    // ���� �ʰ� ����
    if (currentLevel > levelList.size())
    {
        OutputDebugStringA("�̹� �ִ� �����Դϴ�!\n");
        return;
    }

    // ���� ������ ��ȭ ����
    ShopItemLevelData& data = levelList[currentLevel - 1];

    static std::random_device rd;
    static std::mt19937 generator(rd());
    std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

    // ���� ����
    float randValue = distribution(generator);
    bool success = randValue <= data.rate;

    if (success)
    {
        currentLevel++;
        OutputDebugStringA(("��ȭ ����! ���� ����: " + to_string(currentLevel) + "\n").c_str());
    }
    else
    {
        int oldLevel = currentLevel;
        currentLevel = max(1, currentLevel - data.down);
        OutputDebugStringA(("��ȭ ����! ���� ����: " + to_string(oldLevel) + " �� " + to_string(currentLevel) + "\n").c_str());
    }
    ShowUpgradeResultImage(success);
    // UI ������ ����
    int slotIndex = itemID - 1; // buySlots�� itemID�� 1���� ��ġ�Ѵٰ� ����
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
    // ���� ȿ�� ������ ���⼭ �ʿ信 ���� Player���� ����
    // ����:
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

    numberTextures.push_back(L"Resources/Textures/UI/dot.png"); // '.'��
}

void Shop::ChangeShop(int newHermitID)
{
    ClearSlots();

    hermitID = newHermitID;

    // �ؽ�ó ���� �� ���� ����
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

    // --- ?? �߰��� �κ�: ��ȭ ���� UI �ٽ� �ݿ� ---
    if (hermitID == 2)
    {
        unordered_map<int, vector<ShopItemLevelData>>& levelDataMap = ShopManager::Get()->GetShop2ItemLevels();

        for (int itemID = 1; itemID <= 5; ++itemID) // 1~5���� ��ȭ ����
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

        // ?? ������ ����(index == 5, itemID == 6)�� ���� ���θ� �Ǵ��Ͽ� ó��
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

        // RegisterBuyEvent�� index�� �������� �����ϹǷ� index == 5 ����
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

    return 1; // �⺻ ������ 1
}