#pragma once
class Player;
class Ore : public Cube
{
public:
    Ore(int oreID,const string& modelPath);
    ~Ore();

    void Render();
    void Update();
    void Edit();

    void DropItems();
    void TakeDamage(int damage);

    void ResetOre();

    oreData GetData() { return data; }
    int GetHp() { return health; }

    bool IsRayCollision(const Ray& ray, RaycastHit* hit);

    void SetColliderColor(const Float4& color);

private:
    SphereCollider* collider;
    int oreID;
    int health;
    oreData data;
    Model* model;
    Player* player;
    ParticleSystem* particle; 
    float duration = 1.0f;
    float lifeTime = 0.0f;
};