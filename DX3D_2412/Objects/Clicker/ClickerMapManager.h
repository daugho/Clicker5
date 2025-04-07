#pragma once
class Room;
class Player;
class Door;
class TeleportDoor;
class ClickerMapManager : public Singleton<ClickerMapManager>
{
    friend class Singleton;
public:
    ClickerMapManager();
    ~ClickerMapManager();

    void Update();
    void Render();
	void PostRender();
    void Edit();
    void CreateRoom1();

    void CheckCollision();
    Player* GetPlayer() { return player; }
    float GetHeight(const Vector3& pos);

    Room* GetRoom1() { return room1; }

private:
    Player* player;
    OreManager* oreManager;
    //OreInventory* inventory;
    vector<Cube*> roomCubes;
    Cube* floor;
    Room* room1;
    float soundTimer = 0.0f;
    float soundInterval = 5.0f;
};