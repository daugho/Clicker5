#include "Framework.h"

GoldDisplay::GoldDisplay()
{
    // 숫자 텍스처 경로 초기화
    for (int i = 0; i <= 9; i++) {
        textures.push_back(L"Resources/Textures/UI/" + to_wstring(i) + L".png");
    }

    Quad* gIcon = new Quad(L"Resources/Textures/UI/G.png");
    gIcon->SetLocalPosition(position);
    gIcon->SetTag("GoldDigit_G");
    gIcon->Load();
    alpha.push_back(gIcon);

}

GoldDisplay::~GoldDisplay()
{
    for (auto digit : digits)
        delete digit;
}

void GoldDisplay::SetGold(int value)
{
    // 기존 숫자 제거
    for (auto digit : digits)
        delete digit;
    digits.clear();

    string str = to_string(value);

    for (int i = 0; i < str.length(); i++)
    {
        char c = str[i];
        int num = c - '0';
        if (num < 0 || num > 9) continue;

        Quad* digit = new Quad(textures[num]);

        float offsetX = spacing * i;
        digit->SetLocalPosition(position + Vector3(offsetX, 0, 0));
        digit->SetTag("GoldDigit_" + to_string(i));
        digit->Load();
        digits.push_back(digit);
    }
}

void GoldDisplay::SetPosition(const Vector3& pos)
{
    position = pos;

    // 위치 갱신
    float offsetX = 0.0f;
    for (auto digit : digits)
    {
        digit->SetLocalPosition(position + Vector3(offsetX, 0, 0));
        offsetX += spacing;
    }
}

void GoldDisplay::Update()
{
    for (auto digit : digits)
        digit->Update();
    //for (auto digit : alpha)
    //    digit->Update();
}

void GoldDisplay::Render()
{
    for (auto digit : digits)
        digit->Render();
    for (auto digit : alpha)
        digit->Render();
}

void GoldDisplay::Edit()
{
    for (auto digit : digits)
        digit->Edit();
    //for (auto digit : alpha)
    //    digit->Edit();
}

