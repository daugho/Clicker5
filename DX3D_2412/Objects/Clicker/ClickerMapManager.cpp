#include "Framework.h"

ClickerMapManager::ClickerMapManager()
{

	player = new Player();
	player->SetLocalPosition(-3, 2, 1);
	CAM->SetTarget(player);
	CAM->TargetOptionLoad("FPSMode");

	//hermit1 = new Hermit("machine", HermitType::Merchant1);
	//hermit1->SetLocalPosition(Vector3( 2, 1, 2));
	//hermit1->SetLocalScale(Vector3( 2, 2, 2));

	//hermit2 = new Hermit("Bodyfinal", HermitType::Merchant2);
	//hermit2->SetLocalPosition(Vector3(3, 1, 3));
	//hermit2->SetLocalScale(Vector3(2, 2, 2));
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
	room1->AddCube(Vector3(11, 1, 31), Vector3(0, -0.5, 0), baseTexture,Vector2(2,15));
	room1->AddCube(Vector3(1, 6, 31), Vector3(5, 3, 0), baseTexture, Vector2(1,1));  // 오른쪽 벽
	room1->AddCube(Vector3(1, 6, 31), Vector3(-5, 3, 0), baseTexture, Vector2(1, 1)); // 왼쪽 벽
	room1->AddCube(Vector3(11, 6, 1), Vector3(0, 3, -15), baseTexture, Vector2(1, 1)); // 뒷벽
	room1->AddCube(Vector3(11, 6, 1), Vector3(0, 3, 15), baseTexture, Vector2(1, 1));  // 앞벽
	room1->AddCube(Vector3(11, 1, 31), Vector3(0, 6, 0), baseTexture, Vector2(1, 1));  // 천장

	// ?? Room1에 광물 추가
	room1->AddOre(Vector3{2,0,4}, 0);
	room1->AddOre(Vector3{ 2,0, -15 }, 1);
	room1->AddHermit(Vector3(-2.5, 0 , 10), 1);
	room1->AddHermit(Vector3(-2.5, 1 , -10), 2);
	room1->AddBox(Vector3(3, 0, -7));
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
	for (BoxInventory* box : BoxInventoryManager::Get()->GetBoxes())
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
