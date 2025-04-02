#include "Framework.h"

ClickerMapManager::ClickerMapManager()
{

	player = new Player();
	player->SetLocalPosition(-3, 2, 1);
	CAM->SetTarget(player);
	CAM->TargetOptionLoad("FPSMode");
	room1 = new Room();
	
	CreateRoom1();
}

ClickerMapManager::~ClickerMapManager()
{
	delete player;
	delete room1;
}

void ClickerMapManager::Update()
{
	player->Update();
	room1->Update();
	//ShopManager::Get()->Update();
	CheckCollision();
}

void ClickerMapManager::Render()
{
	player->Render();
	room1->Render();
}

void ClickerMapManager::Edit()
{
	//player->Edit();
	room1->Edit();
}

void ClickerMapManager::CreateRoom1()
{
	wstring baseTexture = L"Resources/Textures/Block/T_Ore9_Color.png";
	room1->AddCube(Vector3(20, 1, 30), Vector3(0, -0.5, 0), baseTexture, Vector2(2, 15));

	// 벽 (모두 바닥 기준으로 맞춤)
	room1->AddCube(Vector3(1, 6, 30), Vector3(10, 3, 0), baseTexture, Vector2(15, 1));  // 오른쪽
	room1->AddCube(Vector3(1, 6, 30), Vector3(-10, 3, 0), baseTexture, Vector2(22, 1)); // 왼쪽
	room1->AddCube(Vector3(20, 6, 1), Vector3(0, 3, -15), baseTexture, Vector2(5, 1));  // 뒤쪽
	room1->AddCube(Vector3(20, 6, 1), Vector3(0, 3, 15), baseTexture, Vector2(10, 1));   // 앞쪽

	// 천장 (바닥 기준 위로 6)
	room1->AddCube(Vector3(20, 1, 30), Vector3(0, 5.5f, 0), baseTexture, Vector2(10, 15)); // 천장

	// ?? Room1에 광물 추가
	room1->AddOre(Vector3{7,0,10}, 0);
	room1->AddOre(Vector3{ 5,0, -15 }, 1);
	room1->AddHermit(Vector3(-7, 0 , 10), 1);
	room1->AddHermit(Vector3(-7, 0 , -10), 2);
	room1->AddBox(Vector3(-2, 0, 0));
}

void ClickerMapManager::CreateRoom2()
{

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
