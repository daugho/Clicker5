#include "Framework.h"

Room::Room()
{
    oreManager = OreManager::Get();
}

Room::~Room()
{
    for (Cube* cube : roomCubes)
        delete cube;
    delete oreManager;
}

void Room::AddCube(Vector3 size, Vector3 position, const wstring& texturePath,Vector2 tiling)
{
    Cube* cube = new Cube(size,tiling);
    cube->SetLocalPosition(position);
    cube->GetMaterial()->SetDiffuseMap(texturePath);
    cube->GetMaterial()->SetNormalMap(texturePath.substr(0, texturePath.find(L"_Color")) + L"_Normal.png");
    cube->GetMaterial()->SetSpecularMap(texturePath.substr(0, texturePath.find(L"_Color")) + L"_Mask.png");
    roomCubes.push_back(cube);
}

void Room::AddOre(Vector3 pos, int oreID)
{
    oreManager->CreatOre( pos, oreID);
}

void Room::AddHermit(Vector3 pos, int shopID)
{
    ShopManager::Get()->AddShop(pos, shopID);
}

void Room::Edit()
{
    ShopManager::Get()->Edit();
}

void Room::Render()
{
    for (Cube* cube : roomCubes)
        cube->Render();
    oreManager->Render();
    ShopManager::Get()->Render();
}

void Room::Update()
{
    for (Cube* cube : roomCubes)//6¸é render
        cube->Update();
    oreManager->Update();//±¤¹° render
    ShopManager::Get()->Update();//machine render

}
