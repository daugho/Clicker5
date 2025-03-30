#include "Framework.h"

BoxSlot::BoxSlot() : Button(Vector2(30, 30))
{
    //image = new Quad(Vector2(40, 20));
    //image->SetParent(this);
    //image->SetLocalPosition(Vector3(0, 0, 0.01f));
    //image->GetMaterial()->SetShader(L"Basic/Texture.hlsl");
    image->SetActive(false); // �⺻�� ��Ȱ��
}

BoxSlot::~BoxSlot()
{

}

void BoxSlot::Update()
{
    Button::Update();
    image->Update();
}

void BoxSlot::Render()
{
    Button::Render();
    if (image && hasItem)
        image->Render();

    if (count == 0)
        return;
    if (!ClickerUIManager::Get()->IsBoxOPen())
        return;
    Font::Get()->SetStyle("Icon");
    Font::Get()->SetColor("Black");
    Font::Get()->RenderText(to_string(count), GetGlobalPosition());

}

//void BoxSlot::Edit()
//{
//    image->Edit();
//}

void BoxSlot::SetImage(const wstring& texturePath,int index)
{
    image->SetActive(true);
    image->SetTag("Box" + to_string(index));
    image->GetMaterial()->SetDiffuseMap(texturePath);
    image->Load();
}

void BoxSlot::SetItem(const DropData& data, int count)
{
    this->data = data;
    this->hasItem = true;
    this->count = count;
    image->SetTag("Ore");
    image->SetActive(true);
    image->GetMaterial()->SetDiffuseMap(data.texturePath);  // DropData�� iconPath�� ������ �ִٰ� ����
    image->Load();
}

void BoxSlot::ClearItem()
{
    hasItem = false;
    image->SetActive(false);
}
