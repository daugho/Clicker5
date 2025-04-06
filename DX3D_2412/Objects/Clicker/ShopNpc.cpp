#include "Framework.h"

ShopNPC::ShopNPC(int shopID, Vector3 position) : shopID(shopID)
{
    switch (shopID)
    {
    case 1:
        model = new Model("machine"); // ��: �Ϲ� ����
        break;
    case 2:
        model = new Model("vmachine"); // ��: Ư�� ����
        break;
    default:
        model = new Model("DefaultMerchantModel"); // �⺻ ���� NPC
        break;
    }
    //SetLocalScale(Vector3(1.5f, 3.0f, 1.5f)); // NPC ũ�� ����
    //SetLocalPosition(position);
    model->SetParent(this);
    model->Load();
    model->SetLocalPosition(position);

    collider->SetTag("ShopNPC_Collider");
    collider->SetLocalScale(Vector3(1.5,2.2,1.5));
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
