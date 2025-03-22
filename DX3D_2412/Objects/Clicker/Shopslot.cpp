#include "Framework.h"

ShopSlot::ShopSlot() : Button(Vector2(110, 60)) {
    image->SetActive(false);
}

ShopSlot::~ShopSlot() {}

void ShopSlot::Update() {
    Button::Update();
}

void ShopSlot::Render() {
    Button::Render();
    if (!occupied) return;
    image->Render();
    Font::Get()->RenderText(to_string(item.price) + "G", GetGlobalPosition());
}

void ShopSlot::SetItem(const ShopData& item) {
    this->item = item;
    occupied = true;
    image->SetActive(true);
    image->GetMaterial()->SetDiffuseMap(item.icon);
}

void ShopSlot::OnClick() {
    if (!occupied) return;
    // TODO: 구매 로직 추가
    OutputDebugStringA(("Purchased: " + item.name + "\n").c_str());
}

void ShopSlot::CreatShop()
{
}
