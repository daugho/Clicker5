#pragma once

class GoldDisplay
{
public:
    GoldDisplay();
    ~GoldDisplay();

    void SetGold(int value);  // 골드 값을 설정하면 내부 숫자들을 재구성
    void SetPosition(const Vector3& pos); // UI 위치 지정

    void Update();
    void Render();
    void Edit();

private:
    vector<Quad*> digits;  // 각 숫자 이미지
    vector<wstring> textures; // "0.png" ~ "9.png" 경로들
    vector<Quad*> alpha;

    Vector3 position;      // 기준 위치
    float spacing = 40.0f; // 숫자 간 간격
};