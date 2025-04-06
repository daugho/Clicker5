#pragma once

class OreManager;
struct DropData;
class OreInventory;
class Shop;
class Player : public SphereCollider
{
private:
	const float GRAVITY = 9.8f;
	const float JUMP_POWER = 3.0f;

public:
	Player();
	~Player();

	void Update();
	void Render();
	void PostRender();
	void Edit();
	void SetVelocity(const Vector3& newVelocity);
	void Mining();
	Vector3 GetVelocity();
	SphereCollider* GetCollider() { return collider; }
	void ToggleLight();
	void OpenShop();
	void OpenBox();
	void OpenDoor();
	OreInventory* GetInventory() { return oreInventory; }
	//������ ��� �Լ�
	void ApplyShopDamageBoost();
	void Shop2buypick();

	//ü�� �ӵ� ����Լ�
	void ApplyShopSpeedBoost();

	//���� ����Լ�
	void RecalculateMiningCooldown();
	void RecalculateDamage();

	void SetKey(int value) { Bronzekey = value; }
	bool HasKey() const { return Bronzekey > 0; }
private:
	void Control();

	void Jump();
	void Move();

	void SetCursor();

	void TakeDamageToOre(Ore* targetOre);

private:
	//move
	float moveSpeed = 15.0f;
	float rotSpeed = 1.0f;

	//damage
	int baseDamage = 5;          // �⺻ ä�� ������
	int shopBonus = 0;           // ���� ������ ȿ��
	int equipmentBonus = 1;
	int playerDamage = 0;

	//mining speed
	float baseCooldown = .0f;
	float upgradeCooldownBonus = 0.3f;
	float miningCooldown = 2.5f;  //ä�� ��ٿ� (1.5�ʴ� 1ȸ)
	int miningUpgradeLevel = 0;

	//etc
	Vector3 velocity;

	POINT clientCenterPos;

	LightBuffer::Light* light;

	OreInventory* oreInventory;
	Shop* shopOpne;

	float miningTimer = 0.0f;

	SphereCollider* collider;
	float miningRange = 3.0f;
	bool isLightOn = false;
	bool isInventoryOpen = false;
	bool isShopOpen = false;
	bool isFreeView = false;

	public:
	int Bronzekey = 0;
	int Silverkey = 0;
	int Goldkey = 0;
	int Escapekey = 0;
};