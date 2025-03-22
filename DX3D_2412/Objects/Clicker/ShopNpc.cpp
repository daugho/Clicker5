#include "Framework.h"

ShopNPC::ShopNPC(int shopID, Vector3 position) : shopID(shopID)
{
    switch (shopID)
    {
    case 1:
        model = new Model("machine"); // 예: 일반 상인
        break;
    case 2:
        model = new Model("Merchant2Model"); // 예: 특수 상인
        break;
    default:
        model = new Model("DefaultMerchantModel"); // 기본 상점 NPC
        break;
    }
    SetLocalScale(Vector3(1.5f, 3.0f, 1.5f)); // NPC 크기 설정
    SetLocalPosition(position);

    collider = new BoxCollider(Vector3(1,2,1));
    collider->SetParent(this);
    collider->SetLocalPosition(Vector3(0, 0, 0));
}

ShopNPC::~ShopNPC()
{
    delete model;
}

void ShopNPC::Update()
{
    UpdateWorld();
    model->UpdateWorld();
    collider->Update();
}

void ShopNPC::Render()
{
    model->Render();
    collider->Render();
}
