#include "Framework.h"

OreSlot::OreSlot() : Button(Vector2(60,60))
{
	
	image->SetActive(false);
	//SetActive(false);
}

OreSlot::~OreSlot()
{
}

void OreSlot::Update()
{
	Button::Update();
}

void OreSlot::SetItem(const DropData& item, int count)//슬롯에 몇개가있는지 표시
{
    this->item = item;
    this->count = count;
    occupied = true;
    SetActive(true);

	UpdateData();
}

void OreSlot::Render()
{
	Collider::Render();
	image->Render();

	if (count == 0)
		return;
	if (!ClickerUIManager::Get()->IsInventoryOpen())
		return;
	Font::Get()->SetStyle("Icon");
	Font::Get()->SetColor("Red");
	Font::Get()->RenderText(to_string(count), GetGlobalPosition());
}

void OreSlot::ShowTooltip()
{

}

void OreSlot::Clear()
{
	item = DropData();
	count = 0;
	occupied = false;

	image->SetActive(false);
}

void OreSlot::SetData(DropData data, int index)
{
	image->SetActive(true);
	item = data;
	image->GetMaterial()->SetDiffuseMap(item.texturePath);
	this->index = index;
}

void OreSlot::UpdateData()
{
	if (count == 0)
	{
		image->SetActive(false);
		return;
	}

	image->SetActive(true);
	image->GetMaterial()->SetDiffuseMap(item.texturePath);
	image->GetMaterial()->SetShader(L"Basic/Texture.hlsl");
}
