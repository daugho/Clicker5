#include "Framework.h"

Boxmanager::Boxmanager()
{
}

Boxmanager::~Boxmanager()
{
    for (auto box : boxes)
        delete box;
}

void Boxmanager::Update()
{
    for (auto box : boxes)
        box->Update();
}

void Boxmanager::Render()
{
    for (auto box : boxes)
        box->Render();
}

void Boxmanager::Edit()
{
    for (auto box : boxes)
        box->Edit();
}

void Boxmanager::AddBox(Vector3 pos)
{
    if (boxes.empty())
    {
        BoxInventory* box = new BoxInventory(pos);
        box->SetLocalPosition(pos);
        box->UpdateWorld();
        boxes.push_back(box);
    }
    else
    {
        BoxInventory* box = boxes[0]; // 이미 있는 박스
        box->SetLocalPosition(pos);   // 위치만 변경
        box->UpdateWorld();
    }
}

BoxInventory* Boxmanager::GetClosestBox(const Ray& ray, RaycastHit* hit)
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
