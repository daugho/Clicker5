#pragma once

enum class HermitType {
	Merchant1,
	Merchant2,
	Merchant3,
	Merchant4
};
struct ShopData
{
	int id;
	string name;
	wstring descrip;
	int price;
	wstring icon;
};
enum class ShopSlotType {
	ItemSlot,        // ������ ����
	DescriptionSlot, // ���� ����
	BuySlot,         // ���� ��ư
	SellSlot,		//�Ǹ� ��ư
	DescriptionSlot2
};
struct ShopItemData {
	int id;
	string name;
	wstring texturePath;
	int price;
	wstring icon;
};
struct HermitTable
{
	int id;
	string name;
};
struct ShopItemLevelData
{
	int id;
	int level;
	float value;
	float rate;
	wstring iconPath;
	int down;
};

