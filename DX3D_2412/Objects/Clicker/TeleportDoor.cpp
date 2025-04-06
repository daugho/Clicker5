#include "Framework.h"

int TeleportDoor::teleportDoorCount = 0;

TeleportDoor::TeleportDoor()
{
    tdoor = new Model("door");
	tdoor->SetTag("TeleportDoor"+ to_string(teleportDoorCount));
	tdoor->SetParent(this); // �浹ü ��ü�� �θ� ��
	tdoor->SetLocalPosition(0, 0, 0);
	tdoor->Load();
    OutputDebugStringA("[TeleportDoor] ������\n");
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
    OutputDebugStringA("[TeleportDoor] �÷��̾ �����̵� �Ǿ����ϴ�!\n");
}
