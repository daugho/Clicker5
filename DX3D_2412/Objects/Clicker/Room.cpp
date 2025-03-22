#include "Framework.h"

Room::Room()
{
    oreManager = OreManager::Get();
}

Room::~Room()
{
    for (Cube* cube : roomCubes)
        delete cube;
    for (Shop* shop : shops) // ? 개별 Shop 객체 삭제
        delete shop;
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

void Room::Render()
{
    for (Cube* cube : roomCubes)
        cube->Render();
    oreManager->Render();
    for (Shop* shop : shops)
        shop->Render();
    ShopManager::Get()->Render();
}

void Room::Update()
{
    for (Cube* cube : roomCubes)
        cube->Update();
    for (Shop* shop : shops)
        shop->Update();
    oreManager->Update();
    ShopManager::Get()->Update();

}
