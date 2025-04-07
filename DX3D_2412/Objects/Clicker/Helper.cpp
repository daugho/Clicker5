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
			OutputDebugString(L"[Helper] �ڽ� ����! ������ ���� ����\n");
			CheckBoxAndStoreItems();
			currentState = State::Idle;
			FindOre();
		}
		break;
	
	case State::ManualMove:
		PlayAnimation(1); // ���� �̵��� �ȴ� ���
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
	//	Vector3 destPos = terrain->Picking(); // Ŭ�� ��ġ ���
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
			SetManualPath(path); // ManualMove ���·� ��ȯ
	}
}
void Helper::Move()
{
	if (path.empty())
		return;

	Vector3 dest = path.back();

	Vector3 direction = dest - GetGlobalPosition();
	direction.y = 0.0f;

	if (direction.Magnitude() < 0.2f) // ���� threshold
	{
		path.pop_back(); // ���� ��������
		return;
	}

	velocity = direction.GetNormalized();

	Translate(velocity * moveSpeed * DELTA);

	// ���� ���� �ݿ� (Y�� ����)
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
			OutputDebugString(L"[Helper] ������ �����߽��ϴ�!\n");
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
	//	// ? ��� ���� �����ߴ��� ��� Ȯ��
	//	if (pathIndex >= path.size())
	//	{
	//		if (currentState == State::ManualMove)
	//		{
	//			OutputDebugString(L"[Helper] ���� �̵� �Ϸ� �� ���� ����� ���� Ž�� ����\n");
	//			currentState = State::Idle;
	//			FindOre();
	//		}
	//		else if (currentState == State::MovingToOre && targetOre)
	//		{
	//			OutputDebugString(L"[Helper] ������ �����߽��ϴ�!\n");
	//			targetOre = nullptr;
	//			// NOTE: ���⼱ ���� ��ȯ X �� Mining()�� ���� ����
	//		}
	//		else if (currentState == State::MovingToBox)
	//		{
	//			// ������ �Ÿ��� �Ǵ��ϹǷ� ���⼱ ���� ����
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
			OutputDebugString(L"[FindOre] ��� ���� �Ǵ� ���� ��ġ �� Idle\n");
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
		OutputDebugString(L"[FindOre] targetOre == nullptr �� Idle\n");
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

		// �α� ���
		wstring msg = L"[Helper] �ڽ����� �Ÿ�: " + to_wstring(dist) + L"\n";
		OutputDebugString(msg.c_str());

		if (dist < minDist)
		{
			minDist = dist;
			targetBox = box;
		}
	}

	if (!targetBox)
		return;

	// ���� ó�� ���� (������ �Ÿ��� �ڵ� �Ǵ�)
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

	// ? �߾� �����Ǵ� helperInventory ����
	HelperInventory* inventory = ClickerUIManager::Get()->GetHelperInventory();
	if (!inventory)
	{
		OutputDebugString(L"[Helper] inventory�� �������� �ʽ��ϴ�.\n");
		return;
	}

	// ? �κ��丮�� ���� á���� ���� ����
	if (inventory->IsFull())
	{
		OutputDebugString(L"[Helper] �κ��丮 ���� �� �� ���� ����: MovingToBox\n");
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

		wstring msg = L"[Helper] ���� ä��! ���� ü��: " + to_wstring(targetOre->GetHp()) + L"\n";
		OutputDebugString(msg.c_str());

		if (targetOre->GetHp() <= 0)
		{
			// ? �߾� inventory ����
			targetOre->DropItemsToHelper();

			// ����� �Ŀ��� �κ��丮�� á���� üũ
			if (inventory->IsFull())
			{
				OutputDebugString(L"[Helper] ��� �� �κ��丮 ���� �� �� ���� ����: MovingToBox\n");
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
			OutputDebugString(L"[Helper] �ڽ� �뷮 �������� �Ϻ� ������ ���� ����\n");
			// �ʿ�� �Ϻθ� �����ϴ� ���� ��� ����
			continue;
		}
	}

	inventory->Clear();
	OutputDebugString(L"[Helper] �κ��丮 ��� �Ϸ�\n");
	ClickerUIManager::Get()->GetBoxUI()->SetTargetBox(targetBox);  // Ȥ�� targetBox�� �� �Ѱ�ٸ�
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

