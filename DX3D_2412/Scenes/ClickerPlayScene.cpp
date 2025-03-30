#include "Framework.h"
#include "ClickerPlayScene.h"

ClickerPlayScene::ClickerPlayScene()
{
	Font::Get()->AddStyle("Icon", L"배달의민족 주아", 30);
	Font::Get()->AddColor("Blue",0,0,1);
	ClickerUIManager::Get();
	ClickerMapManager::Get();
    InitTerrainFromFloorCube();

    miner = new Helper();
    miner->SetLocalPosition(Vector3(-2, 0, -2));
    miner->SetTerrain(terrain);
    aStar = new AStar();
    miner->SetAStar(aStar);
    aStar->SetNode(terrain);
}

ClickerPlayScene::~ClickerPlayScene()
{
	ClickerUIManager::Delete();
	ClickerMapManager::Delete();
    delete miner;
    delete aStar;
    delete terrain;
}

void ClickerPlayScene::InitTerrainFromFloorCube()
{
    ClickerMapManager::Get()->CreateRoom1();
    Room* room = ClickerMapManager::Get()->GetRoom1();

    if (!room || room->GetCubes().empty())
    {
        return;
    }

    Cube* floorCube = room->GetCubes()[0];
    Vector3 origin = floorCube->GetGlobalPosition();

    UINT width = 20;
    UINT height = 30;

    terrain = new Terrain();
    terrain->SetSize(width, height);
    terrain->MakeMesh();
    terrain->GetMesh()->CreateMesh();
    terrain->SetLocalPosition(Vector3(0, 0, 0));
    float offsetX = width * 0.5f;
    float offsetZ = height * 0.5f;
    Vector3 adjustedPos = origin - Vector3(offsetX, 0.0f, offsetZ);

    terrain->SetLocalPosition(adjustedPos);
    terrain->UpdateWorld();
    Vector3 test = terrain->GetLocalPosition();

    wstring msg = L"[디버깅] Terrain 위치: " + to_wstring(test.x) + L", " + to_wstring(test.y) + L", " + to_wstring(test.z) + L"\n";
    OutputDebugString(msg.c_str());
}

void ClickerPlayScene::Update()
{
 
    miner->Update();

    aStar->Update();
    ClickerUIManager::Get()->Update();
    ClickerMapManager::Get()->Update();
}

void ClickerPlayScene::PreRender()
{
}

void ClickerPlayScene::Render()
{
	ClickerMapManager::Get()->Render();
    miner->Render();
    aStar->Render();
    terrain->Render();
}

void ClickerPlayScene::PostRender()
{
	ClickerUIManager::Get()->Render();
}

void ClickerPlayScene::GUIRender()
{
	ClickerUIManager::Get()->Edit();
	ClickerMapManager::Get()->Edit();
    miner->Edit();
}
