#include "Framework.h"
#include "Helper.h"

Helper::Helper()
{
	//SetTag("Helper");
	model = new ModelAnimator("Miner");
	model->ReadClip("MinerIdle", 0);
	model->CreateTexture();
	model->PlayClip(0);
	model->Load();
	model->SetParent(this);
}

Helper::~Helper()
{
	delete model;
}

void Helper::Update()
{

	Control();
	Move();
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
		OutputDebugString(L"[Helper] 경로 종료 → Idle\n");
		currentState = State::Idle;
		return;
	}

	Vector3 target = path[pathIndex];
	Vector3 dir = target - GetGlobalPosition();

	if (Vector3::GetLength(dir) < 0.1f)
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

void Helper::SetManualPath(const vector<Vector3>& newPath)
{
	path = newPath;
	pathIndex = 0;
	currentState = State::ManualMove;

}

