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

	//데미지 계산 함수
	void RecalculateDamage();
	void RecalculateMiningCooldown();

	//체굴 속도 계산함수
	void ApplyShopDamageBoost();
	void ApplyShopSpeedBoost();

private:
	void Control();

	void Jump();
	void Move();

	void SetCursor();


private:
	//move
	float moveSpeed = 5.0f;
	float rotSpeed = 1.0f;

	//damage
	int baseDamage = 5;          // 기본 채굴 데미지
	int upgradeBonus = 0;        // 업그레이드로 인한 추가 데미지
	int shopBonus = 0;           // 상점 아이템 효과
	int equipmentBonus = 0;
	int playerDamage = 1;

	//mining speed
	float baseCooldown = 1.0f;
	float upgradeCooldownBonus = 0.0f;
	float shopCooldownBonus = 0.0f;
	float miningCooldown = 2.0f;  //채굴 쿨다운 (1.5초당 1회)

	//etc
	Vector3 velocity;

	POINT clientCenterPos;

	LightBuffer::Light* light;

	OreInventory* oreInventory;
	Shop* shopOpne;

	float miningTimer = 0.0f;

	SphereCollider* collider;
	float miningRange = 3.0f;
	bool isLightOn = true;
	bool isInventoryOpen = false;
	bool isShopOpen = false;
	bool isFreeView = false;
};