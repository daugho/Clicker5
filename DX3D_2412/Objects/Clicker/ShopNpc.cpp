#include "Framework.h"

ShopNPC::ShopNPC(int shopID, Vector3 position) : shopID(shopID)
{
    switch (shopID)
    {
    case 1:
        model = new Model("machine"); // ��: �Ϲ� ����
        break;
    case 2:
        model = new Model("Merchant2Model"); // ��: Ư�� ����
        break;
    default:
        model = new Model("DefaultMerchantModel"); // �⺻ ���� NPC
        break;
    }
    SetLocalScale(Vector3(1.5f, 3.0f, 1.5f)); // NPC ũ�� ����
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
