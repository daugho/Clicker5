#pragma once
class ShopNPC : public Cube
{
public:
    ShopNPC(int shopID, Vector3 position);
    ~ShopNPC();

    void Update();
    void Render();

    int GetID() const { return shopID; }
    Model* GetModel() { return model; }

private:
    Model* model;
    int shopID;
};