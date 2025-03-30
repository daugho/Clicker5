#include "Framework.h"

ShopSlot::ShopSlot() : Button(Vector2(110, 60))
{
    image = new Quad(Vector2(80,80));
    image->SetParent(this);             // 슬롯에 종속
    image->SetLocalPosition(Vector3(0, 0, 0.01f));  // 버튼 위에 살짝 띄움
    image->SetLocalScale(Vector3(0.8f, 0.8f, 1));   // 슬롯보다 약간 작게
    image->UpdateWorld();
    image->SetActive(false);

    levelImage = new Quad(Vector2(30, 30)); // 크기 적당히 조절
    levelImage->SetParent(this);
    levelImage->SetLocalPosition(Vector3(0, 0, 0.01f));  // 위치 조절
    levelImage->SetActive(false);
}

ShopSlot::~ShopSlot() {}

void ShopSlot::Update()
{
    Button::Update();
    levelImage->Update();
}

void ShopSlot::Render()
{
    if (!ClickerUIManager::Get()->IsShopOpen())
        return;
    Button::Render();
    image->Render();
    levelImage->Render();
    if (slotType == ShopSlotType::ItemSlot)
    {
        string text = to_string(item.price) + "G";
        Font::Get()->SetStyle("Icon");
        Font::Get()->SetColor("Black");
        Font::Get()->RenderText(text, GetGlobalPosition() + Vector3(-30, -30, 0));
    }
}

void ShopSlot::SetItem(const ShopData& item, int index) 
{
    this->item = item;
    this->slotIndex = index;
    occupied = true;
    slotType = ShopSlotType::ItemSlot;//정해진 text를 출력하기위해 타입 정하기.
    image->SetActive(true);
    image->SetTag("Ore_ShopSlot_Icon_" + to_string(index));
    image->GetMaterial()->SetDiffuseMap(item.icon);
    image->Load();
}

void ShopSlot::SetDescrip(const ShopData& item, int index)
{
    this->item = item;
    this->slotIndex = index;
    occupied = true;
    slotType = ShopSlotType::DescriptionSlot;//정해진 text를 출력하기위해 타입 정하기.

    image->SetActive(true);
    image->SetTag("Ore_ShopSlot_Icon_" + to_string(index));
    image->GetMaterial()->SetDiffuseMap(item.descrip);
    image->Load();
}

void ShopSlot::SetBuyEvent(const ShopData& item, int index)
{
    this->item = item;
    this->slotIndex = index;
    occupied = true;
    slotType = ShopSlotType::SellSlot;//정해진 text를 출력하기위해 타입 정하기.

    image->SetActive(true);
    image->SetTag("Ore_ShopSlot_Buy_" + to_string(index));
    image->GetMaterial()->SetDiffuseMap(L"Resources/Textures/UI/buybutton.png");
    image->Load();
}

void ShopSlot::SetLevel(const ShopItemLevelData& level, int index)
{
    this->item = item;
    this->slotIndex = index;
    occupied = true;
    slotType = ShopSlotType::ItemLevel;//정해진 text를 출력하기위해 타입 정하기.
    levelImage->SetActive(true);
    levelImage->SetTag("Level" + to_string(index));
    levelImage->GetMaterial()->SetDiffuseMap(level.ratePath);
    levelImage->Load();
}

void ShopSlot::SetSellbutton(const function<void()>& callback)
{
    occupied = true;
    slotType = ShopSlotType::SellSlot;

    image->SetActive(true);
    image->SetTag("Ore_ShopSlot_SellAll");
    image->GetMaterial()->SetDiffuseMap(L"Resources/Textures/UI/sell.png");
    image->Load();

    SetEvent(callback);
}


void ShopSlot::OnClick()
{
    if (!occupied) return;

    OutputDebugStringA(("Purchased: " + item.name + "\n").c_str());

    Shop* parentShop = dynamic_cast<Shop*>(GetParentTransform());
    if (!parentShop) return;

    const int index = slotIndex;

    auto& iconSlots = parentShop->GetIconSlots();
    auto& descSlots = parentShop->GetDescSlots();

    if (index >= 0 && index < iconSlots.size()) {
        iconSlots[index]->GetImage()->SetActive(false);
    }

    if (index >= 0 && index < descSlots.size()) {
        descSlots[index]->GetImage()->SetActive(false);
    }
}

void ShopSlot::Edit()
{
    image->Edit();
    //levelImage->Edit();
}

