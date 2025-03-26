#include "Framework.h"

ShopNPC::ShopNPC(int shopID, Vector3 position) : shopID(shopID)
{
    switch (shopID)
    {
    case 1:
        model = new Model("machine"); // ��: �Ϲ� ����
        break;
    case 2:
        model = new Model("Bodyfinal"); // ��: Ư�� ����
        break;
    default:
        model = new Model("DefaultMerchantModel"); // �⺻ ���� NPC
        break;
    }
    //SetLocalScale(Vector3(1.5f, 3.0f, 1.5f)); // NPC ũ�� ����
    //SetLocalPosition(position);
    model->SetParent(this);
    model->SetLocalPosition(position);
    model->Load();

    collider = new BoxCollider(Vector3(1,2,1));
    collider->SetLocalPosition(position.x,position.y+1,position.z);
    collider->SetParent(this);//machine�� �浹
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
