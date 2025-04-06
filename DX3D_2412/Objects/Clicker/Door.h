#pragma once

enum class KeyType
{
	None,
	Bronze,
	Silver,
	Gold,
	Escape
};

class Door : public BoxCollider
{
public:
	Door();
	~Door();

	void Update();
	void Render();
	void Edit();

	bool IsActivated() const { return activated; }
	void Activate();

	void SetRequiredKey(KeyType key) { requiredKey = key; }
	KeyType GetRequiredKey() const { return requiredKey; }
	void SetRotationAxis(const Vector3& axis) { rotationAxis = axis; }
private:
	Model* door;
	Model* frame;
	bool activated = false;
	float rate = 0.0f;
	float speed = 1.0f;
	KeyType requiredKey = KeyType::None;
	static int doorcounter;
	Vector3 rotationAxis = Vector3(0, 1, 0);
};