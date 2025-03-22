#pragma once

class ClickerPlayScene : public Scene
{
public:
	ClickerPlayScene();
	~ClickerPlayScene();



	// Scene을(를) 통해 상속됨
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