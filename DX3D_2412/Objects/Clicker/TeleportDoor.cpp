#include "Framework.h"

int TeleportDoor::teleportDoorCount = 0;

TeleportDoor::TeleportDoor()
{
    tdoor = new Model("door");
	tdoor->SetTag("TeleportDoor"+ to_string(teleportDoorCount));
	tdoor->SetParent(this); // 충돌체 자체가 부모가 됨
	tdoor->SetLocalPosition(0, 0, 0);
	tdoor->Load();
    OutputDebugStringA("[TeleportDoor] 생성됨\n");
	teleportDoorCount++;
}

TeleportDoor::~TeleportDoor()
{
}

void TeleportDoor::Update()
{
    tdoor->UpdateWorld();
	BoxCollider::Update();
}

void TeleportDoor::Render()
{
	tdoor->Render();
    BoxCollider::Render();
}

void TeleportDoor::Edit()
{
	tdoor->Edit();
	BoxCollider::Edit();
}

void TeleportDoor::SetTargetPosition(const Vector3& pos)
{
    targetPosition = pos;
}

void TeleportDoor::Activate(Player* player)
{
    if (!player) return;

    player->SetLocalPosition(targetPosition);
    OutputDebugStringA("[TeleportDoor] 플레이어가 순간이동 되었습니다!\n");
}
