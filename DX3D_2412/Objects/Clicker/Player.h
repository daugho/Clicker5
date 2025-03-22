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

private:
	void Control();

	void Jump();
	void Move();

	void SetCursor();


private:
	float moveSpeed = 5.0f;
	float rotSpeed = 1.0f;
	int playerDamage = 5;

	Vector3 velocity;

	POINT clientCenterPos;

	LightBuffer::Light* light;

	OreInventory* oreInventory;
	Shop* shopOpne;
	float miningCooldown = 1.0f;  //Ã¤±¼ Äð´Ù¿î (ÃÊ´ç 1È¸)
	float miningTimer = 0.0f;

	SphereCollider* collider;
	float miningRange = 3.0f;
	bool isLightOn = true;
	bool isInventoryOpen = false;
	bool isShopOpen = false;
	bool isFreeView = false;
};