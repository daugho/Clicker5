#include "Framework.h"

BoxInventoryManager::BoxInventoryManager()
{
}

BoxInventoryManager::~BoxInventoryManager()
{
    for (auto box : boxes)
        delete box;
}

void BoxInventoryManager::Update()
{
    for (auto box : boxes)
        box->Update();
}

void BoxInventoryManager::Render()
{
    for (auto box : boxes)
        box->Render();
}

void BoxInventoryManager::Edit()
{
    for (auto box : boxes)
        box->Edit();
}

void BoxInventoryManager::AddBox(Vector3 pos)
{
    BoxInventory* box = new BoxInventory(pos);
    boxes.push_back(box);
}

BoxInventory* BoxInventoryManager::GetClosestBox(const Ray& ray, RaycastHit* hit)
{
    BoxInventory* closest = nullptr;
    float minDist = FLT_MAX;

    for (auto box : boxes)
    {
        RaycastHit tempHit;
        if (box->GetCollider()->IsRayCollision(ray, &tempHit))
        {
            float dist = Vector3::Distance(ray.origin, tempHit.point);
            const float maxRange = 4.0f;
            if (dist < minDist && dist <= maxRange)
            {
                minDist = dist;
                closest = box;
                *hit = tempHit;
            }
        }
    }

    return closest;
}
