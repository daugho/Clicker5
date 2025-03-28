#include "Framework.h"

RateDisplay::RateDisplay()
{
    for (int i = 0; i <= 9; i++)
        textures.push_back(L"Resources/Textures/UI/ShopUI/ratenum/" + to_wstring(i) + L".png");

    textures.push_back(L"Resources/Textures/UI/ShopUI/ratenum/dot.png"); // '.'는 마지막에 추가
}

RateDisplay::~RateDisplay()
{
    for (auto digit : digits)
        delete digit;
}

void RateDisplay::SetRate(float rate)
{
    for (auto digit : digits)
        delete digit;
    digits.clear();

    string rateStr = to_string(rate).substr(0, 4);  // 소수점 아래 2자리까지만
    for (int i = 0; i < rateStr.size(); ++i)
    {
        wchar_t wc = rateStr[i];
        wstring path;

        if (wc >= '0' && wc <= '9')
            path = textures[wc - '0'];
        else if (wc == '.')
            path = L"Resources/Textures/UI/ShopUI/ratenum/dot.png";

        Quad* digit = new Quad(path);
        digit->SetLocalPosition(position + Vector3(i * spacing, 0, 0));
        digit->SetParent(this);
        digit->SetTag("RateLevel_Slot" + to_string(slotID) + "_" + to_string(i));
        digit->Load();
        digits.push_back(digit);
    }
}

void RateDisplay::SetPosition(const Vector3& pos)
{
    position = pos;
    for (int i = 0; i < digits.size(); ++i)
        digits[i]->SetLocalPosition(position + Vector3(i * spacing, 0, 0));
}

void RateDisplay::Update()
{
    for (auto digit : digits)
        digit->Update();
}

void RateDisplay::Render()
{
    for (auto digit : digits)
        digit->Render();
}

void RateDisplay::Edit()
{
    for (auto digit : digits)
        digit->Edit();
}
