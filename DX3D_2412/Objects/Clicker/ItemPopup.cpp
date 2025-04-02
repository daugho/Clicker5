#include "Framework.h"

ItemPopup::ItemPopup() : Quad(L"") // �� �ؽ�ó�� ����
{
    SetLocalPosition(Vector3(CENTER.x, CENTER.y - 150, 0)); // ȭ�� �߾� ��
    SetActive(false); // ó���� ��Ȱ��ȭ
}

ItemPopup::~ItemPopup() {}

void ItemPopup::Play(const wstring& texturePath)
{
    SetTexture(texturePath);
   //GetMaterial()->SetDiffuseMap(texturePath);

    Float4 color = GetMaterial()->GetData()->diffuse;
    color.w = 1.0f;
    GetMaterial()->GetData()->diffuse = color;

    elapsedTime = 0.0f;
    isPlaying = true;
    SetActive(true);
}

void ItemPopup::Update()
{
    if (!isPlaying) return;
    
    elapsedTime += DELTA;
    
    float alpha = 1.0f - (elapsedTime / showTime);
    alpha = max(0.0f, min(1.0f, alpha));
    
    Float4 color = GetMaterial()->GetData()->diffuse;
    color.w = alpha;
    GetMaterial()->GetData()->diffuse = color;
    
    if (elapsedTime >= showTime)
    {
        isPlaying = false;
        SetActive(false);
    }

    Quad::Update();
}

void ItemPopup::Render()
{
    Environment::Get()->SetAlphaBlend(true);
    if (!IsActive()) return;
    Quad::Render();
    Environment::Get()->SetAlphaBlend(false);
}

void ItemPopup::Edit()
{
    Quad::Edit();
}
