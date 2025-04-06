#pragma once

class TeleportDoor : public BoxCollider
{
public:
    TeleportDoor();
    ~TeleportDoor();

	void Update();
	void Render();
	void Edit();

    void SetTargetPosition(const Vector3& pos);
    void Activate(Player* player);  // 플레이어를 텔레포트 시킵니다

private:
	Model* tdoor;
    Vector3 targetPosition;
    static int teleportDoorCount;
};