#include "Framework.h"

BoxInventory::BoxInventory(Vector3 position)
{
    model = new Model("woodbox");
    model->SetParent(this);
    model->SetLocalPosition(position);
    model->Load();
    collider = new BoxCollider(Vector3(2, 1, 1));
    collider->SetLocalPosition(position.x, position.y+0.5, position.z);
    collider->SetParent(this);
}

BoxInventory::~BoxInventory()
{
    delete model;
    delete collider;
}

void BoxInventory::Update()
{
    model->UpdateWorld();
    collider->Update();
}

void BoxInventory::Render()
{
    model->Render();
    collider->Render();
}

void BoxInventory::Edit()
{
    model->Edit();
}

bool BoxInventory::AddItem(const DropData& item, int count)
{
    return false;
}

bool BoxInventory::IsFull() const
{
    return false;
}

int BoxInventory::GetTotalItemCount() const
{
    return 0;
}

void BoxInventory::CreateSlots()
{
}
