#pragma once

class Room
{
public:
    Room();
    ~Room();

    void AddCube(Vector3 size, Vector3 position, const wstring& texturePath,Vector2 tiling);
    void AddOre(Vector3 pos, int oreID);
    void AddHermit(Vector3 pos, int shopID);
    void AddBox(Vector3 pos);
    Door* AddDoor(Vector3 pos);
    void AddTeleportDoor(Vector3 pos, Vector3 target);
    void Edit();
    void Render();
    void Update();

    vector<Cube*>& GetCubes() { return roomCubes; }
    vector<Shop*>& GetHermit() { return shops; }
    vector<Door*>& GetDoors() { return doors; }
    vector<TeleportDoor*>& GetTDoors() { return teleportDoors; }

private:
    vector<Cube*> roomCubes;
    vector<Shop*> shops;
    OreManager* oreManager;
    vector<Door*> doors;
	vector<TeleportDoor*> teleportDoors;
    int DoorColliderCount = 0;
	int teleportDoorColliderCount = 0;
};