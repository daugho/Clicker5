#pragma once

class ItemPopup : public Quad
{
public:
    ItemPopup();
    ~ItemPopup();

    void Update();
    void Render();
    void Edit();
    void Play(const wstring& texturePath); // 아이템 표시 시작

private:
    float showTime = 2.0f; // 0.5초 표시
    float elapsedTime = 0.0f;
    bool isPlaying = false;
};