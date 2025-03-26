	
	//SCENE->Create("Start", new CollisionScene());

	//SCENE->Create("Start", new ModelAnimationScene());
	//SCENE->Create("Start", new ModelEquipScene());
	//SCENE->Create("Start", new AStarScene());
	//SCENE->Create("Start", new RenderTargetScene());
	//SCENE->Create("Start", new BillboardScene());	
	//SCENE->Create("Start", new WeatherScene());	
	//SCENE->Create("Start", new ParticleEditorScene());	
#include "Framework.h"

#include "Scenes/GridScene.h"

#include "Scenes/CollisionScene.h"
#include "Scenes/ModelRenderScene.h"
#include "Scenes/ModelExportScene.h"
#include "Scenes/ModelAnimationScene.h"
#include "Scenes/ModelEquipScene.h"
#include "Scenes/BillboardScene.h"
#include "Scenes/WeatherScene.h"
#include "Scenes/ParticleEditorScene.h"
#include "Scenes/ClickerPlayScene.h"

GameManager::GameManager()
{
	//SCENE->Create("Grid", new GridScene());
	SCENE->Create("Start", new ClickerPlayScene());
	//SCENE->Create("Start", new ModelRenderScene());
	//SCENE->Create("Export", new ModelExportScene());

	//SCENE->Add("Grid");
	SCENE->Add("Start");
	//SCENE->Add("Export");

	Create();
}

GameManager::~GameManager()
{
	Release();
}

void GameManager::Update()
{
	Keyboard::Get()->Update();
	Timer::Get()->Update();

	SCENE->Update();

	Environment::Get()->Update();
}

void GameManager::Render()
{
	SCENE->PreRender();

	Device::Get()->Clear();

	Environment::Get()->SetRender();
	Environment::Get()->SetViewport();
	SCENE->Render();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	Font::Get()->GetDC()->BeginDraw();

	string fps = "FPS : " + to_string(Timer::Get()->GetFPS());
	Font::Get()->RenderText(fps, { 100, SCREEN_HEIGHT - 10 });

	Environment::Get()->SetPostRender();
	SCENE->PostRender();

	Environment::Get()->Edit();
	SCENE->GUIRender();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	Font::Get()->GetDC()->EndDraw();

	Device::Get()->Present();
}

void GameManager::Create()
{
	Device::Get();
	Environment::Get();
	Timer::Get();
	Keyboard::Get();
	SceneManager::Get();
	Font::Get();
	Observer::Get();

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(DEVICE, DC);

	Font::Get()->AddColor("White", 1, 1, 1);
	Font::Get()->AddStyle("Default", L"����ü");

	Font::Get()->SetColor("White");
	Font::Get()->SetStyle("Default");
}

void GameManager::Release()
{
	Device::Delete();	
	Environment::Delete();
	Timer::Delete();
	Keyboard::Delete();
	SceneManager::Delete();
	Font::Delete();
	Observer::Delete();

	Shader::Delete();
	Texture::Delete();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();

	ImGui::DestroyContext();
}

