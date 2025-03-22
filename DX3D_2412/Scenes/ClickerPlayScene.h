#pragma once

class ClickerPlayScene : public Scene
{
public:
	ClickerPlayScene();
	~ClickerPlayScene();



	// Scene��(��) ���� ��ӵ�
	void Update() override;

	void PreRender() override;

	void Render() override;

	void PostRender() override;

	void GUIRender() override;

private:
	Player* player;
	OreManager* oreManager;
	OreInventory* inventory;
};