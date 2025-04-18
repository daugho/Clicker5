#include "Framework.h"

Player::Player()
{
	tag = "Player";

	localPosition.y = radius;

	clientCenterPos = { SCREEN_WIDTH >> 1, SCREEN_HEIGHT >> 1 };
	ClientToScreen(hWnd, &clientCenterPos);
	SetCursorPos(clientCenterPos.x, clientCenterPos.y);
	ShowCursor(false);

	//CAM->SetLocalPosition(0, 0, 0);
	//CAM->SetLocalRotation(0, 0, 0);
	//CAM->SetParent(this);

	light = Environment::Get()->GetLight(0);
	light->type = 2;
	light->color = { 1, 1, 1, 1 };
	light->attentionIntensity = 0.4;
	light->range = 15.0;
	light->isActive = 0;

	//oreInventory = new OreInventory();
	collider = new SphereCollider(1.0f); // 반지름 1.0 설정
	collider->SetParent(this);
	RecalculateDamage();
	Audio::Get()->Add("Pick","Resources/Sounds/Pick.wav");
	Audio::Get()->Add("walk","Resources/Sounds/walk.mp3");
	Audio::Get()->Add("open","Resources/Sounds/dooropen.mp3");
}

Player::~Player()
{
	delete collider;
}

void Player::Update()
{
	if (ClickerUIManager::Get()->IsInventoryOpen() || ClickerUIManager::Get()->IsShopOpen()|| ClickerUIManager::Get()->IsBoxOPen())
		return;
	if (KEY->Down(VK_F2))
	{
		isFreeView = true;
		CAM->SetTarget(nullptr);
		ShowCursor(true);
	}

	if (KEY->Down(VK_F3))
	{
		isFreeView = false;
		CAM->SetTarget(ClickerMapManager::Get()->GetPlayer());
		CAM->TargetOptionLoad("FPSMode");
		ShowCursor(false);
	}
	if (isFreeView)
	{
		return;
	}
	collider->SetLocalPosition(GetLocalPosition());
	collider->UpdateWorld();

	light->position = localPosition;
	light->direction = CAM->GetForward();
	SetCursor();
	Control();
	ToggleLight();
	Jump();
	Move();

	UpdateWorld();
	Mining();
	if (KEY->Down('E'))
	{
		OpenShop();
		OpenBox();
		OpenDoor();
	}
}

void Player::Render()
{
	//Collider::Render();

}

void Player::PostRender()
{
}

void Player::Edit()
{
}

void Player::SetVelocity(const Vector3& newVelocity)
{
	velocity = newVelocity;
}

void Player::Mining()
{
	miningTimer += DELTA;

	if (miningTimer < miningCooldown)
		return;

	Ray ray = CAM->ScreenPointToRay(CENTER);
	RaycastHit hit;

	Ore* closestOre = OreManager::Get()->GetClosestOre(ray, &hit);
	float distance = FLT_MAX;

	if (closestOre)
	{
		distance = Vector3::Distance(hit.point, CAM->GetGlobalPosition());

		if (distance <= miningRange)
		{
			ClickerUIManager::Get()->SetCursorColor(Float4(0, 0 , 1, 1));
		}
		else
		{
			ClickerUIManager::Get()->ResetCursorColor();
		}

		if (KEY->Press(VK_LBUTTON) && distance <= miningRange)
		{
			closestOre->TakeDamage(playerDamage);
			miningTimer = 0.0f;
			Audio::Get()->Play("Pick");
		}
	}
	else
	{
		ClickerUIManager::Get()->ResetCursorColor();
	}
}

//void Player::ToggleInventory()
//{
//	if (KEY->Down('I'))
//	{
//		isInventoryOpen = !isInventoryOpen;
//		oreInventory->SetActive(isInventoryOpen);
//	}
//}
//
//void Player::ToggleUI()
//{
//	if (KEY->Down('I'))
//	{
//		isInventoryOpen = !isInventoryOpen;
//		oreInventory->SetActive(isInventoryOpen);
//	}
//
//	// B 키를 눌러 상점 열기/닫기
//	if (KEY->Down('B'))
//	{
//		isShopOpen = !isShopOpen;
//		shopOpne->SetActive(isShopOpen);
//	}
//}
//
//void Player::ForceOpenShopUI()
//{
//	if (KEY->Down('B'))
//	{
//		isShopOpen = !isShopOpen;
//		shopOpne->SetActive(isShopOpen);
//	}
//}


Vector3 Player::GetVelocity()
{
	return velocity * moveSpeed * DELTA;
}

void Player::ToggleLight()
{
	if (KEY->Down('F'))
	{
		isLightOn = !isLightOn;

		if (isLightOn)
		{
			light->color = { 1, 1, 1, 1 };
			light->isActive = 1;
		}
		else
		{
			light->color = { 0, 0, 0, 0 };
			light->isActive = 0;
		}
	}
}

void Player::OpenShop()
{
	Ray ray = CAM->ScreenPointToRay(CENTER);
	RaycastHit hit;

	ShopNPC* npc = ShopManager::Get()->GetClosestShopID(ray, &hit);

	if (npc && !ClickerUIManager::Get()->IsShopOpen())
	{
		int shopID = npc->GetID();
		ClickerUIManager::Get()->OpenShopUI(shopID);
	}
}

void Player::OpenBox()
{
	Ray ray = CAM->ScreenPointToRay(CENTER);
	RaycastHit hit;
	BoxInventory* box = Boxmanager::Get()->GetClosestBox(ray, &hit);
	if (box != nullptr)
	{
		ClickerUIManager::Get()->GetBoxUI()->SetVisible(true);
		OutputDebugStringA("박스 UI 열림\n");
	}
	if (box && !ClickerUIManager::Get()->IsBoxOPen())
	{
		ClickerUIManager::Get()->GetBoxUI()->SetTargetBox(box);
		ClickerUIManager::Get()->OpenBoxUI();
	}
}

void Player::OpenDoor()
{
	Ray ray = CAM->ScreenPointToRay(CENTER);
	RaycastHit hit;

	// 1. 일반 문 먼저 검사
	for (Door* door : ClickerMapManager::Get()->GetRoom1()->GetDoors())
	{
		if (door->IsRayCollision(ray, &hit))
		{
			float distance = Vector3::Distance(GetGlobalPosition(), hit.point);
			if (distance <= 5.0f)
			{
				KeyType requiredKey = door->GetRequiredKey();
				bool hasKey = false;

				switch (requiredKey)
				{
				case KeyType::None:
					hasKey = true;
					break;
				case KeyType::Bronze:
					hasKey = (Bronzekey > 0);
					break;
				case KeyType::Silver:
					hasKey = (Silverkey > 0);
					break;
				case KeyType::Gold:
					hasKey = (Goldkey > 0);
					break;
				case KeyType::Escape:
					hasKey = (Escapekey > 0);
					break;
				}

				if (hasKey)
				{
					door->Activate();
					Audio::Get()->Play("open");

				}
				else
				{
					// 필요한 열쇠가 없을 때 알림 표시
					ClickerUIManager::Get()->GetItemPopup()->Play(L"Resources/Textures/UI/notice_need_key.png");
				}
			}
		}
	}

	// 2. 텔레포트 문도 별도로 검사
	for (TeleportDoor* teleportDoor : ClickerMapManager::Get()->GetRoom1()->GetTDoors())
	{
		if (teleportDoor->IsRayCollision(ray, &hit))
		{
			float distance = Vector3::Distance(GetGlobalPosition(), hit.point);
			if (distance <= 5.0f)
				teleportDoor->Activate(this);
		}
	}
}

void Player::RecalculateDamage()
{
	playerDamage = (baseDamage + shopBonus) * equipmentBonus;

	string msg = "현재 채굴 데미지: " + to_string(playerDamage) + "\n";
	OutputDebugStringA(msg.c_str());
}

void Player::RecalculateMiningCooldown()
{
	miningCooldown = baseCooldown - upgradeCooldownBonus;
	miningCooldown = max(miningCooldown, 0.1f);
	string msg = "현재 채굴 쿨다운: " + to_string(miningCooldown) + "\n";
	OutputDebugStringA(msg.c_str());
}

void Player::ApplyShopDamageBoost()
{
	shopBonus += 3;
	RecalculateDamage();
}

void Player::Shop2buypick()
{
	equipmentBonus *= 1.5f;
	RecalculateDamage();
}

void Player::ApplyShopSpeedBoost()
{
	miningUpgradeLevel++;
	upgradeCooldownBonus = baseCooldown * (1.0f - pow(0.8f, miningUpgradeLevel));
	RecalculateMiningCooldown();
}

void Player::Control()
{
	Vector3 dir;

	if (KEY->Press('W'))
	{
		dir += GetForward();
	}
	if (KEY->Press('S'))
	{
		dir += GetBack();
	}
	if (KEY->Press('A'))
		dir += GetLeft();
	if (KEY->Press('D'))
		dir += GetRight();

	dir.Normalize();

	velocity.x = dir.x;
	velocity.z = dir.z;

	Vector3 delta = mousePos - CENTER;

	Rotate(Vector3::Up(), delta.x * rotSpeed * DELTA);
	CAM->Rotate(Vector3::Left(), delta.y * rotSpeed * DELTA);

	if (KEY->Down(VK_SPACE))
	{
		velocity.y = JUMP_POWER;
	}
}



void Player::Jump()
{
	velocity.y -= GRAVITY * DELTA;

	float bottomHeight = ClickerMapManager::Get()->GetHeight(localPosition);

	if (velocity.y < 0 && localPosition.y - radius <= bottomHeight)
	{
		velocity.y = 0.0f;
	}
}

void Player::Move()
{

	Translate(velocity * moveSpeed * DELTA);
}

void Player::SetCursor()
{
	//if(UIManager::Get()->)

	SetCursorPos(clientCenterPos.x, clientCenterPos.y);
}

void Player::TakeDamageToOre(Ore* targetOre)
{
	if (!targetOre) return;

	targetOre->TakeDamage(playerDamage);
}

