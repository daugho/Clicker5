#pragma once

class ItemPopup : public Quad
{
public:
    ItemPopup();
    ~ItemPopup();

    void Update();
    void Render();
    void Edit();
    void Play(const wstring& texturePath); // ������ ǥ�� ����

private:
    float showTime = 2.0f; // 0.5�� ǥ��
    float elapsedTime = 0.0f;
    bool isPlaying = false;
};