#include "Framework.h"
#include "ClickerPlayScene.h"

ClickerPlayScene::ClickerPlayScene()
{
    player = new Player();
	Font::Get()->AddStyle("Icon", L"배달의민족 주아", 20);
	Font::Get()->AddStyle("Shop2", L"배달의민족 주아", 20);
	Font::Get()->AddColor("Blue",0,0,1);
	Font::Get()->AddColor("White",1,1,1);
	Font::Get()->AddColor("Black",0,0,0);
	ClickerUIManager::Get();
	ClickerMapManager::Get();
    InitTerrainFromFloorCube();

    miner = nullptr;
    //miner->SetLocalPosition(Vector3(2, 0, 2));
    //miner->SetTerrain(terrain);
    aStar = new AStar();
    //miner->SetAStar(aStar);
    aStar->SetNode(terrain);
    particle = new Rain();
    particle->Play(Vector3(70, 70, 5));
}

ClickerPlayScene::~ClickerPlayScene()
{
	ClickerUIManager::Delete();
	ClickerMapManager::Delete();
    delete miner;
    delete aStar;
    delete terrain;
    delete player;
    delete particle;
}

void ClickerPlayScene::InitTerrainFromFloorCube()
{
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
    particle->Update();
    if (KEY->Down('H') && Shop::hasMiner == 1)
    {
        if (!miner) // 중복 생성 방지
        {
            miner = new Helper();
            miner->SetTerrain(terrain);
            miner->SetAStar(aStar);
            miner->SetLocalPosition(Vector3(2, 0, 2));
            isHelperActive = true;
        }
    }
    if (isHelperActive)
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
    if (isHelperActive)
        miner->Render();
    aStar->Render();
    terrain->Render();
	particle->Render();
}

void ClickerPlayScene::PostRender()
{
	ClickerUIManager::Get()->Render();
	ClickerMapManager::Get()->PostRender();
}

void ClickerPlayScene::GUIRender()
{
	ClickerUIManager::Get()->Edit();
	ClickerMapManager::Get()->Edit();
    //miner->Edit();
    particle->GUIRender();
}
