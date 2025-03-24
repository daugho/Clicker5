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
    //for (ShopNPC* shop : shopNPCs)
    //    shop->Edit();
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
    npc = new ShopNPC(shopID, pos);
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
        LoadShopTable(data.id);
    }
    fclose(file);
}

void ShopManager::LoadShopTable(int hermitID)
{
    string path;

    switch (hermitID)
    {
    case 1:
        path = "Resources/Tables/HermitShop1.csv";
        break;
    case 2:
        path = "Resources/Tables/HermitShop2.csv";
        LoadShop2Table(hermitID);
        break;
    case 3:
        path = "Resources/Tables/HermitShop3.csv";
        break;
    default:
        path = "Resources/Tables/HermitShop_Default.csv";
        break;
    }

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

void ShopManager::LoadShop2Table(int hermitID)
{
    string path = "Resources/Tables/shop2item.csv";

    FILE* file;
    fopen_s(&file, path.c_str(), "r");

    if (!file)
    {
        OutputDebugStringA(("Shop2Item CSV 열기 실패: " + path + "\n").c_str());
        return;
    }

    bool isFirstLine = true;
    while (!feof(file))
    {
        char temp[1024];
        fgets(temp, 1024, file);

        if (isFirstLine) {
            isFirstLine = false;
            continue;
        }

        string row = temp;
        Utility::Replace(row, "\n", "");
        vector<string> datas = Utility::SplitString(row, ",");

        if (datas.size() < 6) continue; // 안전 체크

        ShopItemLevelData data;
        data.id = stoi(datas[0]);
        data.level = stoi(datas[1]);
        data.value = stof(datas[2]);
        data.rate = stof(datas[3]);
        data.iconPath = Utility::ToWString(datas[4]);
        data.down = stoi(datas[5]);

        shop2ItemLevels[data.id].push_back(data);
    }

    fclose(file);

    OutputDebugStringA("Shop2 아이템 CSV 로드 완료\n");
}


