#include "Framework.h"
int Door::doorcounter = 0;

Door::Door()
{
	SetPivot(Vector3(-1.5, 0, 0));
	door = new Model("door");
	door->SetParent(this); // 충돌체 자체가 부모가 됨
	door->SetTag("DoorImage"+ to_string(doorcounter));
	door->SetLocalPosition(0, 0, 0);
	door->SetLocalScale(1.5,1.5,1.5);
	door->Load();
	doorcounter++;
	//door->SetPivot(Vector3( 0, 0,-5));

	//frame = new Model("door_frame");
	//frame->SetParent(this);
	//frame->SetLocalPosition(0, 0, 0);
	//frame->Load();

}

Door::~Door()
{
	delete door;
	//delete collider;
}

void Door::Update()
{
	UpdateWorld();
	door->UpdateWorld();
	//frame->UpdateWorld();
	BoxCollider::Update();
	if (activated && rate < 1.0f)
	{
		rate += DELTA * speed;
		rate = min(rate, 1.0f); // 1.0까지만

		float angle = -XM_PIDIV2 * rate; // 0 ~ 90도 회전

		// 회전축에 따라 회전 적용
		SetLocalRotation(rotationAxis * angle);

		string msg = "[Door] rate: " + to_string(rate) + "\n";
		OutputDebugStringA(msg.c_str());

		if (rate >= 1.0f)
		{
			isActive = false;
		}
	}

	//collider->Update();
}

void Door::Render()
{
	door->Render();
	//frame->Render();
	BoxCollider::Render();
	//collider->Render();
}

void Door::Edit()
{
	door->Edit();
	//frame->Edit();
	BoxCollider::Edit();
	//collider->Edit();
}

void Door::Activate()
{
	if (activated) return;

	activated = true;
	rate = 0.0f;

}

