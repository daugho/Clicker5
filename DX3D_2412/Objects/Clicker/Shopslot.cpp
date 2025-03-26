#include "Framework.h"

ShopSlot::ShopSlot() : Button(Vector2(110, 60))
{
    image = new Quad(Vector2(80,80));
    image->SetParent(this);             // ���Կ� ����
    image->SetLocalPosition(Vector3(0, 0, 0.01f));  // ��ư ���� ��¦ ���
    image->SetLocalScale(Vector3(0.8f, 0.8f, 1));   // ���Ժ��� �ణ �۰�
    image->UpdateWorld();
    image->SetActive(false);
}

ShopSlot::~ShopSlot() {}

void ShopSlot::Update()
{
    Button::Update();
}

void ShopSlot::Render()
{
    if (!ClickerUIManager::Get()->IsShopOpen())
        return;
    Button::Render();
    image->Render();
    if (slotType == ShopSlotType::ItemSlot)
    {
        string text = to_string(item.price) + "G";
        Font::Get()->SetStyle("Icon");
        Font::Get()->SetColor("Black");
        Font::Get()->RenderText(text, GetGlobalPosition() + Vector3(-30, -30, 0));
    }
    if (slotType == ShopSlotType::DescriptionSlot2)
    {
        string currentlevel = to_string(reinforce.level);
        string nextlevel = to_string(reinforce.level + 1);
        string successRate = to_string((int)(reinforce.rate * 100)) + "%";
        string down = to_string(reinforce.down);

        string info = "Lv." + currentlevel + " �� Lv." + nextlevel;
        info += " | ������: " + successRate;
        info += " | ���� �� -" + down + "����";
        Vector3 textPos = GetGlobalPosition() + Vector3(10, 0, 0);
        Font::Get()->SetStyle("Icon");
        Font::Get()->SetColor("Black");
        Font::Get()->RenderText(info, textPos);
    }
}

void ShopSlot::SetItem(const ShopData& item, int index) 
{
    this->item = item;
    this->slotIndex = index;
    occupied = true;
    slotType = ShopSlotType::ItemSlot;//������ text�� ����ϱ����� Ÿ�� ���ϱ�.
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
    slotType = ShopSlotType::DescriptionSlot;//������ text�� ����ϱ����� Ÿ�� ���ϱ�.

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
    slotType = ShopSlotType::SellSlot;//������ text�� ����ϱ����� Ÿ�� ���ϱ�.

    image->SetActive(true);
    image->SetTag("Ore_ShopSlot_Buy_" + to_string(index));
    image->GetMaterial()->SetDiffuseMap(L"Resources/Textures/UI/buybutton.png");
    image->Load();
}

void ShopSlot::SetShop2Descrip(const ShopData& item, int index)
{
    this->item = item;
    this->slotIndex = index;
    occupied = true;
    slotType = ShopSlotType::DescriptionSlot2;
    //image->SetActive(true);
    //image->Load();
}

void ShopSlot::RenderUpgradeInfo()
{
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
}

