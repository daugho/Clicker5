#include "Framework.h"

Room::Room()
{
    oreManager = OreManager::Get();
}

Room::~Room()
{
    for (Cube* cube : roomCubes)
        delete cube;
    for (Door* door : doors)
        delete door;
    for (TeleportDoor* tdoor : teleportDoors)
        delete tdoor;
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

void Room::AddBox(Vector3 pos)
{
    Boxmanager::Get()->AddBox(pos);
}

Door* Room::AddDoor(Vector3 pos)
{
    Door* door = new Door();
    door->SetLocalPosition(pos);
    door->SetLocalScale(1, 1, 1);
	door->SetTag("Door" + to_string(DoorColliderCount));
    door->Load();
    doors.push_back(door);
    DoorColliderCount++;
    return door;
}

void Room::AddTeleportDoor(Vector3 pos, Vector3 target)
{
    TeleportDoor* door = new TeleportDoor();
    door->SetLocalPosition(pos);
    door->SetTargetPosition(target);
	door->SetTag("TeleportDoor_collider" + to_string(teleportDoorColliderCount));
    door->Load();
    teleportDoors.push_back(door);
    teleportDoorColliderCount++;
}

void Room::Edit()
{
	//ShopManager::Get()->Edit();
    //ShopManager::Get()->Edit();
    //Boxmanager::Get()->Edit();
    //for (Door* door : doors)
    //    door->Edit();
	//for (TeleportDoor* tdoor : teleportDoors)
	//	tdoor->Edit();
	//for (Cube* cube : roomCubes)
	//	cube->Edit();
	for (Shop* shop : shops)
		shop->Edit();
	//for (BoxInventory* box : Boxmanager::Get()->GetBoxes())
	//    box->Edit();
}

void Room::Render()
{
    for (Cube* cube : roomCubes)
        cube->Render();
    for (Door* door : doors)
        door->Render();
	for (TeleportDoor* tdoor : teleportDoors)
		tdoor->Render();
    oreManager->Render();
    ShopManager::Get()->Render();
    Boxmanager::Get()->Render();
}

void Room::Update()
{
    for (Cube* cube : roomCubes)//6¸é render
        cube->Update();
    for (Door* tdoor : doors)
        tdoor->Update();
	for (TeleportDoor* door : teleportDoors)
		door->Update();
    oreManager->Update();//±¤¹° render
    ShopManager::Get()->Update();//machine render
    Boxmanager::Get()->Update();
}
