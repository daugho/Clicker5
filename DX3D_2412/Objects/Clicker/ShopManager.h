#pragma once
#include "HermitData.h"
class ShopNPC;
class CapsuleCollider;
class ShopManager : public Singleton<ShopManager>
{
public:
	ShopManager();
	~ShopManager();

	void Render() ; 
	void Update() ;
	void Edit();

	void AddShop(Vector3 pos, int shopID);
	void LoadHermitData(string path);
	void LoadShopTable(int hermitID);

	ShopNPC* GetClosestShopID(const Ray& ray, RaycastHit* hit);

	vector<ShopNPC*>& GetShops() { return shopNPCs; }
	vector<ShopData>& GetShopData(int hermitID);
private:
	unordered_map<int, vector<ShopData>> shopTables;
	unordered_map<int, string> hermitNames;
	unordered_map<int, HermitTable> hermitTable;

	vector<ShopNPC*> shopNPCs;
};