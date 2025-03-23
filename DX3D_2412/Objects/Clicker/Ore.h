#pragma once
class Player;
class Ore : public Sphere
{
public:
    Ore(int oreID,const string& modelPath);
    ~Ore();

    void Render();
    void Update();
    void Edit();

    void DropItems();
    void TakeDamage(int damage);

    oreData GetData() { return data; }
    int GetHp() { return health; }

    bool IsRayCollision(const Ray& ray, RaycastHit* hit);

    void SetColliderColor(const Float4& color);
    void SetData(const oreData& data);

private:

    int oreID;
    int health;
    oreData data;
    Model* model;
    Player* player;
    ParticleSystem* particle; 
    float duration = 1.0f;
    float lifeTime = 0.0f;
};