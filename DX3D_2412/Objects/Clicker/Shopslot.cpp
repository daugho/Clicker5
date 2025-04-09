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
        Font::Get()->SetColor("White");
        Font::Get()->RenderText(text, GetGlobalPosition() + Vector3(160, 0, 0));
    }
    if (slotType == ShopSlotType::ItemSlot2)
    {
        string text = to_string(item.price) + "G";
        Font::Get()->SetStyle("Shop2");
        Font::Get()->SetColor("White");
        Font::Get()->RenderText(text, GetGlobalPosition() + Vector3(30, -35, 0));
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
    image->SetTexture(item.icon);
    //image->GetMaterial()->SetDiffuseMap(item.icon);
    image->Load();
}

void ShopSlot::SetItem2(const ShopData& item, int index)
{
	this->item = item;
	this->slotIndex = index;
	occupied = true;
	slotType = ShopSlotType::ItemSlot2;//정해진 text를 출력하기위해 타입 정하기.
	image->SetActive(true);
	image->SetTag("Ore_ShopSlot2_Icon_" + to_string(index));
	image->SetTexture(item.icon);
	//image->GetMaterial()->SetDiffuseMap(item.icon);
	image->Load();
}

void ShopSlot::SetDescrip(const ShopData& item, int index)
{
    this->item = item;
    this->slotIndex = index;
    occupied = true;
    slotType = ShopSlotType::DescriptionSlot;//정해진 text를 출력하기위해 타입 정하기.

    image->SetActive(true);
    image->SetTag("Ore_ShopSlot_Descrip_" + to_string(index));
    image->SetTexture(item.descrip);
    //image->GetMaterial()->SetDiffuseMap(item.descrip);
    image->Load();
}

void ShopSlot::SetDescrip2(const ShopData& item, int index)
{
	this->item = item;
	this->slotIndex = index;
	occupied = true;
	slotType = ShopSlotType::DescriptionSlot2;//정해진 text를 출력하기위해 타입 정하기.
	image->SetActive(true);
	image->SetTag("Ore_ShopSlot2_Descrip__" + to_string(index));
	image->SetTexture(item.descrip);
	//image->GetMaterial()->GetData()->diffuse = Float4(0,0,0,0);
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
    image->SetTexture(L"Resources/Textures/UI/buybutton.png");
    //image->GetMaterial()->SetDiffuseMap(L"Resources/Textures/UI/buybutton.png");
    image->Load();
}

void ShopSlot::SetBuyEvent2(const ShopData& item, int index)
{
    this->item = item;
    this->slotIndex = index;
    occupied = true;
    slotType = ShopSlotType::SellSlot;//정해진 text를 출력하기위해 타입 정하기.

    image->SetActive(true);
    image->SetTag("Ore_ShopSlot2_Buy_" + to_string(index));
    image->SetTexture(L"Resources/Textures/UI/buybutton2.png");
    //image->GetMaterial()->SetDiffuseMap(L"Resources/Textures/UI/buybutton.png");
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
    levelImage->SetTexture(level.ratePath);
    //levelImage->GetMaterial()->SetDiffuseMap(level.ratePath);
    levelImage->Load();
}

void ShopSlot::SetSellbutton(const function<void()>& callback)
{
    occupied = true;
    slotType = ShopSlotType::SellSlot;

    image->SetActive(true);
    image->SetTag("Ore_ShopSlot_SellAll");
    image->SetTexture(L"Resources/Textures/UI/sell.png");
    //image->GetMaterial()->SetDiffuseMap(L"Resources/Textures/UI/sell.png");
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

