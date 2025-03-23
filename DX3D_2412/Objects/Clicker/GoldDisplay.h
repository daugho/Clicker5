#pragma once

class GoldDisplay
{
public:
    GoldDisplay();
    ~GoldDisplay();

    void SetGold(int value);  // ��� ���� �����ϸ� ���� ���ڵ��� �籸��
    void SetPosition(const Vector3& pos); // UI ��ġ ����

    void Update();
    void Render();
    void Edit();

private:
    vector<Quad*> digits;  // �� ���� �̹���
    vector<wstring> textures; // "0.png" ~ "9.png" ��ε�
    vector<Quad*> alpha;

    Vector3 position;      // ���� ��ġ
    float spacing = 40.0f; // ���� �� ����
};