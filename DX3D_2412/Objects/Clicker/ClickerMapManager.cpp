#include "Framework.h"

ClickerMapManager::ClickerMapManager()
{

	player = new Player();
	player->SetLocalPosition(-3, 2, 1);
	CAM->SetTarget(player);
	CAM->TargetOptionLoad("FPSMode");
	room1 = new Room();
	CreateRoom1();
	Audio::Get()->Add("waterfall1", "Resources/Sounds/waterfall1.mp3");
	Audio::Get()->Add("waterfall2", "Resources/Sounds/waterfall2.mp3");
}

ClickerMapManager::~ClickerMapManager()
{
	delete player;
	delete room1;
	//delete room2;
}

void ClickerMapManager::Update()
{
	soundTimer += DELTA;

	if (soundTimer >= soundInterval)
	{
		int randomIndex = rand() % 2;
		if (randomIndex == 0)
			Audio::Get()->Play("waterfall1");
		else
			Audio::Get()->Play("waterfall2");

		soundTimer = 0.0f;
	}
	player->Update();
	room1->Update();
	ShopManager::Get()->Update();
	CheckCollision();
	OreManager::Get()->Update();
}

void ClickerMapManager::Render()
{
	player->Render();
	room1->Render();
}

void ClickerMapManager::PostRender()
{
	OreManager::Get()->PostRender();
}

void ClickerMapManager::Edit()
{
	//player->Edit();
	room1->Edit();
	//room2->Edit();
}

void ClickerMapManager::CreateRoom1()
{

	wstring baseTexture = L"Resources/Textures/Block/T_Ore9_Color.png";
	wstring	passageTexture = L"Resources/Textures/Block/T_Ore10_Color.png";
	room1->AddCube(Vector3(20, 1, 30), Vector3(0, -0.5, 0), baseTexture, Vector2(10, 15));

	// 벽 
	room1->AddCube(Vector3(1, 6, 27), Vector3(10, 3, 4), baseTexture, Vector2(14, 3));  // 오른쪽
	room1->AddCube(Vector3(1, 6, 30), Vector3(-10, 3, 0), baseTexture, Vector2(15, 3)); // 왼쪽
	room1->AddCube(Vector3(20, 6, 1), Vector3(0, 3, -15), baseTexture, Vector2(10, 3));  // 뒤쪽
	room1->AddCube(Vector3(20, 6, 1), Vector3(0, 3, 15), baseTexture, Vector2(10, 3));   // 앞쪽

	// 천장 
	room1->AddCube(Vector3(20, 1, 30), Vector3(0, 5.5f, 0), baseTexture, Vector2(10, 15)); // 천장

	//  Room1에 광물 추가
	room1->AddOre(Vector3{0,0,10}, 0);
	room1->AddOre(Vector3{ -2, 0, -10 }, 1);
	room1->AddOre(Vector3{ 40 , 0, 50 }, 2);
	room1->AddOre(Vector3{ 65 , 0, 50 }, 3);
	room1->AddOre(Vector3{ 55 , 0, 60 }, 4);

	room1->AddHermit(Vector3(8, 0 , -5), 1);
	room1->AddHermit(Vector3(8, 0 , -2), 2);

	room1->AddBox(Vector3(-2, 0, 0));

	Door* door1 = room1->AddDoor(Vector3(10, 0, -10));
	door1->SetRequiredKey(KeyType::Bronze);

	//Door* door2 = room1->AddDoor(Vector3(30, 0, 40));
	//door2->SetRotationAxis(Vector3(-1, 0, 1));
	//door2->SetRequiredKey(KeyType::Silver);

	room1->AddTeleportDoor(Vector3(26, 2, -10), Vector3(33, 1, 51));

	room1->AddTeleportDoor(Vector3(30, 2, 50), Vector3(27, 1, -11));
	room1->AddTeleportDoor(Vector3(50, 2, 70), Vector3(27, 1, -11));

	//통로 바닥
	room1->AddCube(Vector3(20, 1, 10), Vector3(20, -0.5, -10), passageTexture, Vector2(15, 1));
	//
	Vector3 floorSize = Vector3(20, 1, 10);
	Vector3 floorPos = Vector3(20, -0.5f, -10);
	float roomHeight = 6.0f;
	float wallThickness = 1.0f;
	Vector3 center = floorPos + Vector3(0, roomHeight / 2.0f, 0);
	room1->AddCube(
		Vector3(floorSize.x, wallThickness, floorSize.z),
		center + Vector3(0, roomHeight / 2.0f, 0),
		passageTexture, Vector2(1, 1)
	);

	// 왼쪽 벽
	//room1->AddCube(
	//	Vector3(wallThickness, roomHeight, floorSize.z),
	//	center + Vector3(-floorSize.x / 2.0f + wallThickness / 2.0f, 0, 0),
	//	passageTexture, Vector2(1, 1)
	//);

	// 오른쪽 벽
	room1->AddCube(
		Vector3(wallThickness, roomHeight, floorSize.z),
		center + Vector3(floorSize.x / 2.0f - wallThickness / 2.0f, 0, 0),
		passageTexture, Vector2(1, 1)
	);

	// 앞쪽 벽
	room1->AddCube(
		Vector3(floorSize.x, roomHeight, wallThickness),
		center + Vector3(0, 0, floorSize.z / 2.0f - wallThickness / 2.0f),
		passageTexture, Vector2(1, 1)
	);

	// 뒤쪽 벽
	room1->AddCube(
		Vector3(floorSize.x, roomHeight, wallThickness),
		center + Vector3(0, 0, -floorSize.z / 2.0f + wallThickness / 2.0f),
		passageTexture, Vector2(1, 1)
	);
	////////////////////////////////////////////////////////////////////////////////////////////2nd
	wstring	passageTexture2 = L"Resources/Textures/Block/T_Ore11_Color.png";
	room1->AddCube(Vector3(40, 1, 40), Vector3(50, -0.5, 50), passageTexture2, Vector2(20, 20));
	Vector3 floorSize2 = Vector3(40, 1, 40);
	Vector3 floorPos2 = Vector3(50, -0.5f, 50);
	float roomHeight2 = 10.0f;
	// 중심 위치 계산 (바닥에서 절반 높이 위로)
	Vector3 center2 = floorPos2 + Vector3(0, roomHeight2 / 2.0f, 0);

	// 바닥
	room1->AddCube(floorSize2, floorPos2, passageTexture2, Vector2(1, 1));

	// 천장
	room1->AddCube(
		Vector3(floorSize2.x, wallThickness, floorSize2.z),
		center2 + Vector3(0, roomHeight2 / 2.0f, 0),
		passageTexture2, Vector2(floorSize.x, floorSize.z)
	);

	// 왼쪽 벽
	room1->AddCube(
		Vector3(wallThickness, roomHeight2, floorSize2.z),
		center2 + Vector3(-floorSize2.x / 2.0f + wallThickness / 2.0f, 0, 0),
		passageTexture2, Vector2(20, 10)
	);

	// 오른쪽 벽
	room1->AddCube(
		Vector3(wallThickness, roomHeight2, floorSize2.z),
		center2 + Vector3(floorSize2.x / 2.0f - wallThickness / 2.0f, 0, 0),
		passageTexture2, Vector2(20, 10)
	);

	// 앞쪽 벽
	room1->AddCube(
		Vector3(floorSize2.x, roomHeight2, wallThickness),
		center2 + Vector3(0, 0, floorSize2.z / 2.0f - wallThickness / 2.0f),
		passageTexture2, Vector2(floorSize.x, roomHeight)
	);

	// 뒤쪽 벽
	room1->AddCube(
		Vector3(floorSize2.x, roomHeight2, wallThickness),
		center2 + Vector3(0, 0, -floorSize2.z / 2.0f + wallThickness / 2.0f),
		passageTexture2, Vector2(floorSize.x, roomHeight)
	);
}


void ClickerMapManager::CheckCollision()
{
	RaycastHit hit;


	for (ShopNPC* npc : ShopManager::Get()->GetShops()) 
	{
		if (npc->GetCollider()->PushSphere(player, &hit)) { // ? NPC와 충돌 감지
			player->SetLocalPosition(player->GetGlobalPosition() + (hit.point * hit.distance));
		}
	}

	// ?? Room 내 모든 Cube(벽, 바닥 등)와 충돌 감지
	for (Cube* cube : room1->GetCubes())
	{
		if (cube->GetCollider()->PushSphere(player, &hit))
		{
			player->SetLocalPosition(player->GetGlobalPosition() + (hit.point * hit.distance));
		}
	}
	for (Ore* ore : OreManager::Get()->GetOres())
	{
		if (ore->GetCollider()->PushSphere(player, &hit))
		{
			player->SetLocalPosition(player->GetGlobalPosition() + (hit.point * hit.distance));
		}
	}
	for (BoxInventory* box : Boxmanager::Get()->GetBoxes())
	{
		if (box->GetCollider()->PushSphere(player, &hit))
		{
			player->SetLocalPosition(player->GetGlobalPosition() + (hit.point * hit.distance));
		}
	}
	for (Door* door : room1->GetDoors())
	{
		if (door->PushSphere(player, &hit))
		{
			player->SetLocalPosition(player->GetGlobalPosition() + (hit.point * hit.distance));
		}
	}	for (TeleportDoor* door : room1->GetTDoors())
	{
		if (door->PushSphere(player, &hit))
		{
			player->SetLocalPosition(player->GetGlobalPosition() + (hit.point * hit.distance));
		}
	}
}

float ClickerMapManager::GetHeight(const Vector3& pos)
{
	Ray ray(pos, Vector3::Down());
	RaycastHit hit;

	float maxHeight = 0.0f;

	for (Cube* block : roomCubes)
	{
		if (block->GetCollider()->IsRayCollision(ray, &hit))
		{
			if (hit.point.y > maxHeight)
				maxHeight = hit.point.y;
		}
	}

	return maxHeight;
}
