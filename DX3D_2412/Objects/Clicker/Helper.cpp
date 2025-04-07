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
	model->Load();
	model->SetParent(this);
	//FindOre();
	//helperInventory = new HelperInventory();
	Audio::Get()->Add("hPick", "Resources/Sounds/Pick.wav");
}

Helper::~Helper()
{
	delete model;
	//delete helperInventory;
}

void Helper::Update()
{
	Rotate();
	UpdateWorld();
	model->Update();

	switch (currentState)
	{
	case State::Idle:
		PlayAnimation(1);
		if (!targetOre || !targetOre->IsActive() || path.empty())
			FindOre();
		break;
	
	case State::MovingToOre:
		if (!isMining)
			PlayAnimation(0);
		MoveAlongPath();
		Rotate();
		Mining();
		break;
	
	case State::MovingToBox:
		PlayAnimation(2);
		MoveAlongPath();
		Rotate();
		if (distanceToBox <= 1.0f)
		{
			OutputDebugString(L"[Helper] 박스 도착! 아이템 저장 실행\n");
			CheckBoxAndStoreItems();
			currentState = State::Idle;
			FindOre();
		}
		break;
	
	case State::ManualMove:
		PlayAnimation(1); // 수동 이동도 걷는 모션
		MoveAlongPath();
		Rotate();
		break;
	}
	UpdateDistanceToBox();
	//FindBox();
	//Move();
	Control();


}

void Helper::Render()
{
	model->Render();
	Collider::Render();
}

void Helper::Edit()
{
	//model->Edit();
	//SphereCollider::Edit();
}

void Helper::Control()
{
	//if (KEY->Down(VK_RBUTTON))
	//{
	//	Vector3 destPos = terrain->Picking(); // 클릭 위치 얻기
	//
	//	int start = aStar->FindCloseNode(GetGlobalPosition());
	//	int end = aStar->FindCloseNode(destPos);
	//
	//	vector<Vector3> path;
	//	aStar->GetPath(start, end, path);
	//
	//	if (!path.empty())
	//		SetManualPath(path);
	//}

	if (KEY->Down(VK_RBUTTON))
	{
		Vector3 destPos = terrain->Picking();

		int start = aStar->FindCloseNode(GetGlobalPosition());
		int end = aStar->FindCloseNode(destPos);

		vector<Vector3> path;
		aStar->GetPath(start, end, path);

		if (!path.empty())
			SetManualPath(path); // ManualMove 상태로 전환
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
	velocity = dir;
	Translate(dir * moveSpeed * DELTA);
	//if (pathIndex >= path.size())
	//	return;
	//
	//Vector3 target = path[pathIndex];
	//Vector3 dir = target - GetGlobalPosition();
	//
	//if (Vector3::GetLength(dir) < 0.1f)
	//{
	//	pathIndex++;
	//
	//	// ? 경로 끝에 도달했는지 즉시 확인
	//	if (pathIndex >= path.size())
	//	{
	//		if (currentState == State::ManualMove)
	//		{
	//			OutputDebugString(L"[Helper] 수동 이동 완료 → 가장 가까운 광물 탐색 시작\n");
	//			currentState = State::Idle;
	//			FindOre();
	//		}
	//		else if (currentState == State::MovingToOre && targetOre)
	//		{
	//			OutputDebugString(L"[Helper] 광물에 도착했습니다!\n");
	//			targetOre = nullptr;
	//			// NOTE: 여기선 상태 전환 X → Mining()이 상태 유지
	//		}
	//		else if (currentState == State::MovingToBox)
	//		{
	//			// 도착은 거리로 판단하므로 여기선 유지 가능
	//			FindBox();
	//		}
	//
	//		return;
	//	}
	//
	//	return;
	//}
	//
	//dir.Normalize();
	//Translate(dir * moveSpeed * DELTA);
}

void Helper::FindOre()
{
	Rotate();
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
	Rotate();
	targetBox = nullptr;
	float minDist = FLT_MAX;

	for (BoxInventory* box : Boxmanager::Get()->GetBoxes())
	{
		float dist = Vector3::Distance(GetGlobalPosition(), box->GetGlobalPosition());

		// 로그 출력
		wstring msg = L"[Helper] 박스까지 거리: " + to_wstring(dist) + L"\n";
		OutputDebugString(msg.c_str());

		if (dist < minDist)
		{
			minDist = dist;
			targetBox = box;
		}
	}

	if (!targetBox)
		return;

	// 도착 처리 생략 (이제는 거리로 자동 판단)
	int start = aStar->FindCloseNode(GetGlobalPosition());
	int end = aStar->FindCloseNode(targetBox->GetGlobalPosition());

	path.clear();
	aStar->GetPath(start, end, path);
	pathIndex = 0;

	currentState = State::MovingToBox;
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
	//PlayAnimation(2);

	if (miningTimer >= miningCooldown)
	{
		Audio::Get()->Play("hPick");
		PlayAnimation(1);
		targetOre->TakeDamageFroHelper(miningDamage); 
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
}

void Helper::CheckBoxAndStoreItems()
{
	HelperInventory* inventory = ClickerUIManager::Get()->GetHelperInventory();

	const auto& items = inventory->GetItems();

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

	inventory->Clear();
	OutputDebugString(L"[Helper] 인벤토리 비움 완료\n");
	ClickerUIManager::Get()->GetBoxUI()->SetTargetBox(targetBox);  // 혹시 targetBox를 안 넘겼다면
	ClickerUIManager::Get()->GetBoxUI()->UpdateSlots();
}

void Helper::UpdateDistanceToBox()
{
	if (targetBox)
		distanceToBox = Vector3::Distance(GetGlobalPosition(), targetBox->GetGlobalPosition());
	else
		distanceToBox = FLT_MAX;
}

void Helper::PlayAnimation(int clipIndex)
{
	if (clipIndex != currentClipIndex)
	{
		model->PlayClip(clipIndex);
		currentClipIndex = clipIndex;
	}
}

void Helper::SetManualPath(const vector<Vector3>& newPath)
{
	path = newPath;
	pathIndex = 0;
	currentState = State::ManualMove;
}

