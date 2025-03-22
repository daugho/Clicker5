#include "Framework.h"

ClickerUIManager::ClickerUIManager()
{
    CreateUI();

    inventory = new OreInventory();
    inventory->SetActive(false);
    shopOpen = new Shop(1);
    shopOpen->SetActive(false);
}

ClickerUIManager::~ClickerUIManager()
{
    delete cursor;
    delete inventory;
}

void ClickerUIManager::Update()
{
    HandleUIToggle();
    ToogleFreeView();
    if (isInventoryOpen)
        inventory->Update();

    if (isShopOpen)
        shopOpen->Update();
}

void ClickerUIManager::Render()
{
    //Environment::Get()->SetAlphaBlend(true);

    cursor->Render();
    inventory->Render();
    shopOpen->Render();
}

void ClickerUIManager::Edit()
{
    inventory->Edit();
    shopOpen->Edit();
}

void ClickerUIManager::ToggleInventory()
{
   // isInventoryOpen = !isInventoryOpen;
   // inventory->SetActive(isInventoryOpen);
   //
   // if (isInventoryOpen)
   // {
   //     ShowCursor(true); // ? 마우스 커서 활성화
   //     //CAM->IsActive(true); // ? 카메라 움직임 잠금
   // }
   // else
   // {
   //     ShowCursor(false); // ? 마우스 커서 비활성화
   //    // CAM->IsActive(false); // ? 카메라 움직임 해제
   // }
}

void ClickerUIManager::ToogleFreeView()
{
   //if (KEY->Down(VK_F2))
   //{
   //    isFreeView = true;
   //    CAM->SetTarget(nullptr);
   //    ShowCursor(true);
   //}
   //
   //if (KEY->Down(VK_F3))
   //{
   //    isFreeView = false;
   //    CAM->SetTarget(ClickerMapManager::Get()->GetPlayer());
   //    CAM->TargetOptionLoad("FPSMode");
   //    ShowCursor(false);
   //}
   //if (isFreeView)
   //{
   //    return;
   //}
}

void ClickerUIManager::SetCursorColor(const Float4& color)
{
    cursor->GetMaterial()->GetData()->diffuse = color;
}

void ClickerUIManager::ResetCursorColor()
{
    cursor->GetMaterial()->GetData()->diffuse = originalCursorColor;
}

void ClickerUIManager::OpenShopUI(int shopID)
{
    shopOpen = new Shop(shopID);
    shopOpen->SetActive(true);

    currentShopID = shopID;
    isShopOpen = true;

    ShowCursor(true);
}

void ClickerUIManager::CloseShopUI()
{
    currentShopID = -1;
    isShopOpen = false;

    ShowCursor(false);
}

void ClickerUIManager::SetCurrentShop(Shop* shop)
{

}

void ClickerUIManager::CreateUI()
{
    cursor = new Quad(L"Resources/Textures/UI/cursor.png");
    cursor->SetLocalPosition(CENTER);
    cursor->UpdateWorld();    
}

void ClickerUIManager::HandleUIToggle()
{
    if (KEY->Down('I'))
    {
        isInventoryOpen = !isInventoryOpen;
        ShowCursor(isInventoryOpen);
        inventory->SetActive(isInventoryOpen);
    }

    if (KEY->Down('B'))
    {
        isShopOpen = !isShopOpen;
        ShowCursor(isShopOpen);
        shopOpen->SetActive(isShopOpen);
    }
}
