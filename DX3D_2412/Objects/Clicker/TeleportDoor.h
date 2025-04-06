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
    void Activate(Player* player);  // �÷��̾ �ڷ���Ʈ ��ŵ�ϴ�

private:
	Model* tdoor;
    Vector3 targetPosition;
    static int teleportDoorCount;
};