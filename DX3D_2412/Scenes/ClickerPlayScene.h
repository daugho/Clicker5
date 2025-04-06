#pragma once

class ClickerPlayScene : public Scene
{
public:
	ClickerPlayScene();
	~ClickerPlayScene();

	void InitTerrainFromFloorCube();

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
	Terrain* terrain;
	Helper* miner;
	AStar* aStar;
	bool isHelperManuallyMoving = false;
	bool isHelperActive = false;
	Particle* particle;
};