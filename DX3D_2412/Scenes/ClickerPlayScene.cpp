#include "Framework.h"
#include "ClickerPlayScene.h"

ClickerPlayScene::ClickerPlayScene()
{
	Font::Get()->AddStyle("Icon", L"����ǹ��� �־�", 30);
	Font::Get()->AddColor("Blue",0,0,1);
	ClickerUIManager::Get();
	ClickerMapManager::Get();
}

ClickerPlayScene::~ClickerPlayScene()
{

	ClickerUIManager::Delete();
	ClickerMapManager::Delete();

}

void ClickerPlayScene::Update()
{

	ClickerUIManager::Get()->Update();
	ClickerMapManager::Get()->Update();

}

void ClickerPlayScene::PreRender()
{
}

void ClickerPlayScene::Render()
{
	ClickerMapManager::Get()->Render();

}

void ClickerPlayScene::PostRender()
{
	ClickerUIManager::Get()->Render();
	//player->PostRender();
}

void ClickerPlayScene::GUIRender()
{
	ClickerUIManager::Get()->Edit();
	ClickerMapManager::Get()->Edit();
}
