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
		Vector3 destPos = terrain->Picking(); // Ŭ�� ��ġ ���

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

	// ������ �Ÿ��̸� �ٷ� ���� ó��
	if (minDist <= 1.0f)
	{
		OutputDebugString(L"[Helper] �ڽ��� ������� �ٷ� ���� �� ���� ����\n");
		CheckBoxAndStoreItems();
		currentState = State::Idle;
		return;
	}

	//���� �ָ� ��� ���� �̵�
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
	//		OutputDebugString(L"[FindBox] Helper�� �̹� �ڽ��� ������ �ֽ��ϴ� �� Idle\n");
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
	//	OutputDebugString(L"[FindBox] targetBox == nullptr �� Idle\n");
	//	currentState = State::Idle;
	//}
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

	if (miningTimer >= miningCooldown)
	{
		targetOre->TakeDamageFroHelper(miningDamage);  // ��Ÿ ������ Ȯ��
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
	//	wstring msg = L"[Helper] ���� ä��! ���� ü��: " + to_wstring(targetOre->GetHp()) + L"\n";
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
			OutputDebugString(L"[Helper] �ڽ� �뷮 �������� �Ϻ� ������ ���� ����\n");
			// �ʿ�� �Ϻθ� �����ϴ� ���� ��� ����
			continue;
		}
	}

	helperInventory->Clear();
	OutputDebugString(L"[Helper] �κ��丮 ��� �Ϸ�\n");
}

void Helper::SetManualPath(const vector<Vector3>& newPath)
{
	path = newPath;
	pathIndex = 0;
	currentState = State::ManualMove;
}

