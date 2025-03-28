#pragma once

class RateDisplay : public Transform
{
public:
    RateDisplay();
    ~RateDisplay();

    void SetRate(float rate);
    void SetPosition(const Vector3& pos);
    void Update();
    void Render();
    void Edit();
    void SetSlotID(int id) { slotID = id; }
private:
    vector<Quad*> digits;
    vector<wstring> textures;
    Vector3 position;
    float spacing = 20.0f;
    int slotID = 0;
};
