#include "Framework.h"

ClickerUIManager::ClickerUIManager()
{
    CreateUI();

    inventory = new OreInventory();
    inventory->SetActive(false);

    goldDisplay = new GoldDisplay();
    goldDisplay->SetPosition(Vector3(50, 50, 0)); // 화면 우측 상단 등 적절한 위치
    goldDisplay->SetGold(0);

    shopOpen = new Shop(1, inventory,goldDisplay);
    shopOpen->SetActive(false);

    shopOpen = new Shop(2, inventory, goldDisplay);
    shopOpen->SetActive(false);

    itemPopup = new ItemPopup();

    boxUI = new BoxUI();
    boxUI->SetActive(false);
}

ClickerUIManager::~ClickerUIManager()
{
    delete cursor;
    delete inventory;
    delete shopOpen;
    delete boxUI;
}

void ClickerUIManager::Update()
{
    goldDisplay->Update();
    itemPopup->Update();
    boxUI->Update();
    HandleUIInput();
    ToogleFreeView();
    UpdateCurrentUI();
}

void ClickerUIManager::Render()
{
    cursor->Render();
    inventory->Render();
    shopOpen->Render();
    goldDisplay->Render();
    itemPopup->Render();
    boxUI->Render();
}

void ClickerUIManager::Edit()
{
    //inventory->Edit();
    //shopOpen->Edit();
    //goldDisplay->Edit();
    //itemPopup->Edit();
    boxUI->Edit();
}

void ClickerUIManager::HandleUIInput()
{
    if (KEY->Down('I') && currentUIState == UIState::None)
        OpenInventory();
    if (KEY->Down('B') && currentUIState == UIState::None)
        OpenShopUI(1); // 현재 예시로 1번 상점 고정
    if (KEY->Down('c') && currentUIState == UIState::None)
        OpenShopUI(2); // 현재 예시로 1번 상점 고정
    if (KEY->Down(VK_ESCAPE) && currentUIState != UIState::None)
        CloseCurrentUI();
}

void ClickerUIManager::OpenInventory()
{
    currentUIState = UIState::Inventory;
    inventory->SetActive(true);
    ShowCursor(true);
}

void ClickerUIManager::OpenShopUI(int shopID)
{
    currentShopID = shopID;
    currentUIState = UIState::Shop;
    shopOpen->ChangeShop(shopID);
    shopOpen->SetActive(true);
    ShowCursor(true);
}

void ClickerUIManager::CloseCurrentUI()
{
    switch (currentUIState)
    {
    case UIState::Inventory:
        inventory->SetActive(false);
        break;
    case UIState::Shop:
        shopOpen->SetActive(false);
        break;
    case UIState::Box:
        boxUI->SetActive(false);
    default:
        break;
    }

    currentUIState = UIState::None;
    ShowCursor(false);
    currentShopID = -1;
}

void ClickerUIManager::OpenBoxUI()
{
    currentUIState = UIState::Box;
    boxUI->SetActive(true);
    ShowCursor(true);
}

void ClickerUIManager::UpdateCurrentUI()
{
    switch (currentUIState)
    {
    case UIState::Inventory:
        inventory->Update();
        break;
    case UIState::Shop:
        shopOpen->Update();
        break;
    default:
        break;
    }
}

void ClickerUIManager::ToogleFreeView()
{
    if (KEY->Down(VK_F2))
    {
        isFreeView = true;
        CAM->SetTarget(nullptr);
        ShowCursor(true);
    }

    if (KEY->Down(VK_F3))
    {
        isFreeView = false;
        CAM->SetTarget(ClickerMapManager::Get()->GetPlayer());
        CAM->TargetOptionLoad("FPSMode");
        ShowCursor(false);
    }

    if (isFreeView)
        return;
}

void ClickerUIManager::SetCursorColor(const Float4& color)
{
    cursor->GetMaterial()->GetData()->diffuse = color;
}

void ClickerUIManager::ResetCursorColor()
{
    cursor->GetMaterial()->GetData()->diffuse = originalCursorColor;
}

void ClickerUIManager::CreateUI()
{
    cursor = new Quad(L"Resources/Textures/UI/cursor.png");
    cursor->SetLocalPosition(CENTER);
    cursor->UpdateWorld();
}