#pragma once

class ShopSlot;

class Shop : public Quad {
public:
    Shop(int hermitID);
    ~Shop();

    void Update();
    void Render();
    void Edit();
    void CreateSlots();
    void CreateSlots2();
    void CreateSlots3();
    bool IsActive() const { return isActive; }

private:
    vector<ShopSlot*> slots;
    bool isActive = false;
    wstring texturePath;
};

