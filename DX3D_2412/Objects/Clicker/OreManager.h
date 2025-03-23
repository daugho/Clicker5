#pragma once

struct oreData
{
    int id;
    string name;
    wstring texturePath;
    wstring shaderPath;
    wstring normalPath;
    Vector3 size;
    int health;
};

struct DropData
{
    int id;
    string name;
    wstring texturePath;
    float dropRate;
    int minCount;
    int maxCount;
    int price;
    wstring rarePath;
};

class OreManager : public Singleton<OreManager>
{
    friend class Singleton;
    friend class Ore;
public:
    OreManager();
    ~OreManager();
public:
    void Update();
    void Render();
    void CreatOre(Vector3 pos, int oreID);
    void LoadOreTable(string path);
    void LoadDropTable(int oreID);

    oreData GetOreData(int oreID) { return oreTable[oreID];}
    vector<DropData>& GetDropData(int oreID) { return dropTable[oreID];}

    Ore* GetClosestOre(const Ray& ray, RaycastHit* hit);
    list<Ore*>& GetOres() { return ores; }


private:
    //Ore* ore;
    unordered_map<int, oreData> oreTable;
    unordered_map<int, vector<DropData>> dropTable;

    list<Ore*> ores;
    map<int, Ore*> oredata;

};