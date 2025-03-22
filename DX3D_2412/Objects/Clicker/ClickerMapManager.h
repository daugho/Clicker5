#pragma once
class Room;
class Player;
class ClickerMapManager : public Singleton<ClickerMapManager>
{
    friend class Singleton;
public:
    ClickerMapManager();
    ~ClickerMapManager();

    void Update();
    void Render();
    void Edit();
    void CreateRoom1();
    void CreateRoom2();
    void CheckCollision();
    Player* GetPlayer() { return player; }
    float GetHeight(const Vector3& pos);

private:
    Player* player;
    OreManager* oreManager;
    //OreInventory* inventory;
    vector<Cube*> roomCubes;
    Cube* floor;
    vector<BoxCollider*> wallColliders;
    vector<BoxCollider*> floorColliders;
    Room* room1;

};