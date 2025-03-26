#include "Framework.h"
#include "BoxUI.h"

BoxUI::BoxUI() : Quad(L"Resources/Textures/UI/woodbox.png")
{
    localPosition = CENTER;
    UpdateWorld();
    CreatSlot();
}

BoxUI::~BoxUI()
{
    //for (OreSlot* slot : slots) {
    //    delete slot;
    //}
}

void BoxUI::Update()
{
    Quad::Update();
    UpdateWorld();
    //for (OreSlot* slot : slots) {
    //    slot->Update();
    //}
}

void BoxUI::Render()
{
    Quad::Render();

    //for (OreSlot* slot : slots) {
    //    slot->Render();
    //}
}

void BoxUI::Edit()
{
}

void BoxUI::CreatSlot()
{
}
