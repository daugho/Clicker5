#include "Framework.h"

ShopManager::ShopManager()
{
    LoadHermitData("Resources/Tables/HermitData.csv");
}

ShopManager::~ShopManager()
{
    for (ShopNPC* shop : shopNPCs)
        delete shop;
}

void ShopManager::Render()
{
    for (ShopNPC* shop : shopNPCs)
        shop->Render();
}

void ShopManager::Update()
{
    for (ShopNPC* shop : shopNPCs)
        shop->Update();
}

void ShopManager::Edit()
{

}


ShopNPC* ShopManager::GetClosestShopID(const Ray& ray, RaycastHit* hit) {
    ShopNPC* closestNPC = nullptr;
    float minDistance = FLT_MAX;

    for (ShopNPC* npc : shopNPCs)
    {
        RaycastHit tempHit;
        if (npc->GetCollider()->IsRayCollision(ray, &tempHit))
        {
            float distance = Vector3::Distance(ray.origin, tempHit.point);
            const float maxRange = 5.0f; // 원하는 범위 설정

            if (distance < minDistance && distance <= maxRange)
            {
                minDistance = distance;
                closestNPC = npc;
                *hit = tempHit;
            }
        }
    }

    return closestNPC;
}

vector<ShopData>& ShopManager::GetShopData(int hermitID)
{
    return shopTables.at(hermitID);
}

void ShopManager::AddShop(Vector3 pos, int shopID)
{
    ShopNPC* npc = new ShopNPC(shopID, pos);
    shopNPCs.push_back(npc);
}

void ShopManager::LoadHermitData(string path)
{
    FILE* file;
    fopen_s(&file, path.c_str(), "r");

    if (!file) { return; }

    bool isFirstLine = true;

    while (!feof(file)) {
        char temp[1024];
        fgets(temp, 1024, file);

        if (isFirstLine) {
            isFirstLine = false;
            continue;
        }

        string row = temp;
        Utility::Replace(row, "\n", "");

        vector<string> datas = Utility::SplitString(row, ",");

        HermitTable data;
        data.id = stoi(datas[0]);
        data.name = datas[1];

        hermitTable[data.id] = data;
    }
    fclose(file);
}

void ShopManager::LoadShopTable(int hermitID)
{
    string path = "Resources/Tables/Ore" + to_string(hermitID) + "DropTable.csv";

    FILE* file;
    fopen_s(&file, path.c_str(), "r");

    if (!file) {
        printf("Failed to open ShopTable file: %s\n", path.c_str());
        return;
    }

    bool isFirstLine = true;
    while (!feof(file)) {
        char temp[1024];
        fgets(temp, 1024, file);

        if (isFirstLine) {
            isFirstLine = false;
            continue;
        }

        string row = temp;
        Utility::Replace(row, "\n", "");
        vector<string> datas = Utility::SplitString(row, ",");

        ShopData data;
        data.id = stoi(datas[0]);
        data.name = datas[1];
        data.descrip = Utility::ToWString(datas[2]);
        data.price = stoi(datas[3]);
        data.icon = Utility::ToWString(datas[4]);

        shopTables[hermitID].push_back(data);  // Hermit ID별로 저장
    }
    fclose(file);
}


