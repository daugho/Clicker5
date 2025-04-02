#include "Framework.h"
#include "Helper.h"

Helper::Helper()
{
	//SetTag("Helper");
	model = new ModelAnimator("Miner");
	model->SetShader(L"Model/Model.hlsl");
	model->ReadClip("MinerIdle", 0);
	model->ReadClip("MinerIdle", 1);
	model->ReadClip("MinerIdle", 2);
	model->CreateTexture();
	//model->PlayClip(2);
	model->Load();
	model->SetParent(this);
	
	//helperInventory = new HelperInventory();
}

Helper::~Helper()
{
	delete model;
	delete helperInventory;
}

void Helper::Update()
{
	switch (currentState)
	{
	case State::Idle:
		FindOre();
		break;

	case State::MovingToOre:
		MoveAlongPath();
		Mining();
		break;

	case State::MovingToBox:
		MoveAlongPath();
		break;

	case State::ManualMove:
		MoveAlongPath();
		break;
	}
	//FindBox();
	//Move();
	Control();
	Rotate();
	UpdateWorld();
	model->Update();
}

void Helper::Render()
{
	model->Render();
	Collider::Render();
}

void Helper::Edit()
{
	model->Edit();
	SphereCollider::Edit();
}

void Helper::Control()
{
	if (KEY->Down(VK_RBUTTON))
	{
		Vector3 destPos = terrain->Picking(); // 클릭 위치 얻기

		int start = aStar->FindCloseNode(GetGlobalPosition());
		int end = aStar->FindCloseNode(destPos);

		vector<Vector3> path;
		aStar->GetPath(start, end, path);

		if (!path.empty())
			SetManualPath(path);
	}
}
void Helper::Move()
{
	if (path.empty())
		return;

	Vector3 dest = path.back();

	Vector3 direction = dest - GetGlobalPosition();
	direction.y = 0.0f;

	if (direction.Magnitude() < 0.2f) // 도착 threshold
	{
		path.pop_back(); // 다음 지점으로
		return;
	}

	velocity = direction.GetNormalized();

	Translate(velocity * moveSpeed * DELTA);

	// 지형 높이 반영 (Y값 보정)
	localPosition.y = terrain->GetHeight(localPosition);
}
void Helper::Rotate()
{
	Vector3 forward = GetForward();
	Vector3 cross = Vector3::Cross(forward, velocity);

	if (cross.y > 0)
		Transform::Rotate(Vector3::Up(), rotSpeed * DELTA);
	else if (cross.y < 0)
		Transform::Rotate(Vector3::Down(), rotSpeed * DELTA);
}

void Helper::MoveAlongPath()
{
	if (pathIndex >= path.size())
	{
		if (currentState == State::MovingToOre && targetOre)
		{
			OutputDebugString(L"[Helper] 광물에 도착했습니다!\n");
			targetOre = nullptr;
		}

		currentState = State::Idle;
		return;
	}

	Vector3 target = path[pathIndex];
	Vector3 dir = target - GetGlobalPosition();

	if (Vector3::GetLength(dir) < 0.f)
	{
		pathIndex++;
		return;
	}

	dir.Normalize();
	Translate(dir * moveSpeed * DELTA);
}

void Helper::FindOre()
{
	targetOre = nullptr;
	float minDist = FLT_MAX;

	for (Ore* ore : OreManager::Get()->GetOres())
	{
		if (!ore->IsActive()) continue;

		float dist = Vector3::Distance(GetGlobalPosition(), ore->GetGlobalPosition());
		if (dist < minDist)
		{
			minDist = dist;
			targetOre = ore;
		}
	}

	if (targetOre)
	{
		int start = aStar->FindCloseNode(GetGlobalPosition());
		int end = aStar->FindCloseNode(targetOre->GetGlobalPosition());

		path.clear();
		aStar->GetPath(start, end, path);

		if (start == end || path.empty())
		{
			OutputDebugString(L"[FindOre] 경로 없음 또는 같은 위치 → Idle\n");
			currentState = State::Idle;
			return;
		}

		wchar_t buffer[100];
		swprintf_s(buffer, 100, L"[FindOre] Path size: %d\n", (int)path.size());
		OutputDebugString(buffer);

		pathIndex = 0;
		currentState = State::MovingToOre;
	}
	else
	{
		OutputDebugString(L"[FindOre] targetOre == nullptr → Idle\n");
		currentState = State::Idle;
	}
}

void Helper::FindBox()
{
	targetBox = nullptr;
	float minDist = FLT_MAX;

	for (BoxInventory* box : Boxmanager::Get()->GetBoxes())
	{
		float dist = Vector3::Distance(GetGlobalPosition(), box->GetGlobalPosition());

		if (dist < minDist)
		{
			minDist = dist;
			targetBox = box;

		}
	}

	if (!targetBox)
		return;

	// 도착한 거리이면 바로 저장 처리
	if (minDist <= 1.0f)
	{
		OutputDebugString(L"[Helper] 박스가 가까워서 바로 도착 → 저장 실행\n");
		CheckBoxAndStoreItems();
		currentState = State::Idle;
		return;
	}

	//아직 멀면 경로 따라 이동
	int start = aStar->FindCloseNode(GetGlobalPosition());
	int end = aStar->FindCloseNode(targetBox->GetGlobalPosition());

	path.clear();
	aStar->GetPath(start, end, path);
	pathIndex = 0;

	currentState = State::MovingToBox;
	//targetBox = nullptr;
	//float minDist = FLT_MAX;
	//
	//for (BoxInventory* box : Boxmanager::Get()->GetBoxes())
	//{
	//	float dist = Vector3::Distance(GetGlobalPosition(), box->GetGlobalPosition());
	//	if (dist < minDist)
	//	{
	//		minDist = dist;
	//		targetBox = box;
	//	}
	//}
	//
	//if (targetBox)
	//{
	//	int start = aStar->FindCloseNode(GetGlobalPosition());
	//	int end = aStar->FindCloseNode(targetBox->GetGlobalPosition());
	//	path.clear();
	//	aStar->GetPath(start, end, path);
	//
	//	if (path.empty())
	//	{
	//		OutputDebugString(L"[FindBox] Helper가 이미 박스에 도착해 있습니다 → Idle\n");
	//		currentState = State::Idle;
	//		return;
	//	}
	//
	//	wchar_t buffer[100];
	//	swprintf_s(buffer, 100, L"[FindBox] Path size: %d\n", (int)path.size());
	//	OutputDebugString(buffer);
	//
	//	pathIndex = 0;
	//	currentState = State::MovingToBox;
	//}
	//else
	//{
	//	OutputDebugString(L"[FindBox] targetBox == nullptr → Idle\n");
	//	currentState = State::Idle;
	//}
}

void Helper::Mining()
{
	if (!targetOre || !targetOre->IsActive())
		return;

	// ? 중앙 관리되는 helperInventory 접근
	HelperInventory* inventory = ClickerUIManager::Get()->GetHelperInventory();
	if (!inventory)
	{
		OutputDebugString(L"[Helper] inventory가 존재하지 않습니다.\n");
		return;
	}

	// ? 인벤토리가 가득 찼으면 상태 변경
	if (inventory->IsFull())
	{
		OutputDebugString(L"[Helper] 인벤토리 가득 참 → 상태 변경: MovingToBox\n");
		currentState = State::MovingToBox;
		FindBox();
		return;
	}

	miningTimer += DELTA;

	float distance = Vector3::Distance(GetGlobalPosition(), targetOre->GetGlobalPosition());
	if (distance > miningRange)
		return;

	if (miningTimer >= miningCooldown)
	{
		targetOre->TakeDamageFroHelper(miningDamage);  // 오타 없도록 확인
		miningTimer = 0.0f;

		wstring msg = L"[Helper] 광물 채굴! 현재 체력: " + to_wstring(targetOre->GetHp()) + L"\n";
		OutputDebugString(msg.c_str());

		if (targetOre->GetHp() <= 0)
		{
			// ? 중앙 inventory 전달
			targetOre->DropItemsToHelper();

			// 드롭한 후에도 인벤토리가 찼는지 체크
			if (inventory->IsFull())
			{
				OutputDebugString(L"[Helper] 드롭 후 인벤토리 가득 참 → 상태 변경: MovingToBox\n");
				currentState = State::MovingToBox;
				FindBox();
			}
		}
	}
	//if (!targetOre || !targetOre->IsActive())
	//	return;
	//
	//miningTimer += DELTA;
	//
	//float distance = Vector3::Distance(GetGlobalPosition(), targetOre->GetGlobalPosition());
	//
	//if (distance > miningRange)
	//	return;
	//
	//if (miningTimer >= miningCooldown)
	//{
	//	targetOre->TakeDamage(miningDamage);
	//	miningTimer = 0.0f;
	//
	//	wstring msg = L"[Helper] 광물 채굴! 현재 체력: " + to_wstring(targetOre->GetHp()) + L"\n";
	//	OutputDebugString(msg.c_str());
	//}
}

void Helper::CheckBoxAndStoreItems()
{
	if (!targetBox || !helperInventory) return;

	const auto& items = helperInventory->GetItems();

	for (const auto& pair : items)
	{
		const DropData& item = pair.first;
		int count = pair.second;

		if (!targetBox->AddItem(item, count))
		{
			OutputDebugString(L"[Helper] 박스 용량 부족으로 일부 아이템 저장 실패\n");
			// 필요시 일부만 저장하는 로직 고려 가능
			continue;
		}
	}

	helperInventory->Clear();
	OutputDebugString(L"[Helper] 인벤토리 비움 완료\n");
}

void Helper::SetManualPath(const vector<Vector3>& newPath)
{
	path = newPath;
	pathIndex = 0;
	currentState = State::ManualMove;
}

