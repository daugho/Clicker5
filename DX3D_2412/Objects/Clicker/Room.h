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
    void Edit();
    void Render();
    void Update();

    vector<Cube*>& GetCubes() { return roomCubes; }
    vector<Shop*>& GetHermit() { return shops; }

private:
    vector<Cube*> roomCubes;
    vector<Shop*> shops;
    OreManager* oreManager;
};