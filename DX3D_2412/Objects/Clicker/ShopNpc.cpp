#include "Framework.h"

ShopNPC::ShopNPC(int shopID, Vector3 position) : shopID(shopID)
{
    switch (shopID)
    {
    case 1:
        model = new Model("machine"); // 예: 일반 상인
        break;
    case 2:
        model = new Model("vmachine"); // 예: 특수 상인
        break;
    default:
        model = new Model("DefaultMerchantModel"); // 기본 상점 NPC
        break;
    }
    //SetLocalScale(Vector3(1.5f, 3.0f, 1.5f)); // NPC 크기 설정
    //SetLocalPosition(position);
    model->SetParent(this);
    model->Load();
    model->SetLocalPosition(position);

    collider->SetTag("ShopNPC_Collider");
    collider->SetLocalScale(Vector3(1.5,2.2,1.5));
    collider->SetLocalPosition(position.x,position.y+1,position.z);
    collider->SetParent(this);//machine의 충돌
}

ShopNPC::~ShopNPC()
{
    delete model;
    delete collider;
}

void ShopNPC::Update()
{
    model->UpdateWorld();
    collider->Update();
    UpdateWorld();
}

void ShopNPC::Render()
{
    model->Render();
    collider->Render();
}

void ShopNPC::Edit()
{
    model->Edit();
}
