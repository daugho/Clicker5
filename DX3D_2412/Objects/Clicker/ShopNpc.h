#pragma once
class ShopNPC : public Cube
{
public:
    ShopNPC(int shopID, Vector3 position);
    ~ShopNPC();

    void Update();
    void Render();
    void Edit();
    int GetID() const { return shopID; }

private:
    Model* model;
    int shopID;
};