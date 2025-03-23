#pragma once

class SphereCollider;

class Sphere : public GameObject<VertexUVNormalTangent>
{
private:
	typedef VertexUVNormalTangent VertexType;

public:
	Sphere(float radius = 1.0f, UINT sliceCount = 32, UINT stackCount = 16);	

	void Update();
	void Render();

	SphereCollider* GetCollider() { return collider; }

	float Radius() { return radius; }

private:
	void MakeMesh();	

private:
	float radius;
	UINT sliceCount, stackCount;

protected:
	SphereCollider* collider;
};