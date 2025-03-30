#pragma once
class Button;
class BoxUI : public Quad
{
public:
	BoxUI();
	~BoxUI();

	void Update();
	void Render();
	void Edit();
	void CreatSlot();
	void SetVisible(bool visible);

	void SetTargetBox(BoxInventory* box);
	void UpdateSlots();
private:
	BoxSlot* insertButton = nullptr;
	BoxSlot* removeButton = nullptr;
	bool isVisible = false;
	vector<BoxSlot*> boxslots;
	BoxInventory* targetBox = nullptr;
};