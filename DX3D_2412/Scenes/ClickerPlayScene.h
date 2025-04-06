#pragma once

class ClickerPlayScene : public Scene
{
public:
	ClickerPlayScene();
	~ClickerPlayScene();

	void InitTerrainFromFloorCube();

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
	Terrain* terrain;
	Helper* miner;
	AStar* aStar;
	bool isHelperManuallyMoving = false;
	bool isHelperActive = false;
	Particle* particle;
};