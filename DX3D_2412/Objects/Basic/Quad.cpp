#include "Framework.h"

Quad::Quad(Vector2 size) : size(size)
{
    tag = "Quad";

    material->SetShader(L"Basic/Texture.hlsl");
    material->SetDiffuseMap(L"Resources/Textures/Colors/White.png");

    vector<VertexUV>& vertices = mesh->GetVertices();
    vertices.emplace_back(-0.5f * size.x, +0.5f * size.y, 0.0f, 0, 0);//0
    vertices.emplace_back(+0.5f * size.x, +0.5f * size.y, 0.0f, 1, 0);//1
    vertices.emplace_back(-0.5f * size.x, -0.5f * size.y, 0.0f, 0, 1);//2
    vertices.emplace_back(+0.5f * size.x, -0.5f * size.y, 0.0f, 1, 1);//3

    vector<UINT>& indices = mesh->GetIndices();
    indices = {
        0, 1, 2, 2, 1, 3,
    };

    mesh->CreateMesh();
}

Quad::Quad(wstring texture)
{
    tag = "Quad";

    material->SetShader(L"Basic/Texture.hlsl");
    material->SetDiffuseMap(texture);
    size = material->GetDiffuseMap()->GetSize();

    vector<VertexUV>& vertices = mesh->GetVertices();
    vertices.emplace_back(-0.5f * size.x, +0.5f * size.y, 0.0f, 0, 0);//0
    vertices.emplace_back(+0.5f * size.x, +0.5f * size.y, 0.0f, 1, 0);//1
    vertices.emplace_back(-0.5f * size.x, -0.5f * size.y, 0.0f, 0, 1);//2
    vertices.emplace_back(+0.5f * size.x, -0.5f * size.y, 0.0f, 1, 1);//3    

    vector<UINT>& indices = mesh->GetIndices();
    indices = {
        0, 1, 2, 2, 1, 3,
    };

    mesh->CreateMesh();
}

void Quad::SetTexture(wstring textureFile)
{
    material->SetDiffuseMap(textureFile);

    //localScale = material->GetDiffuseMap()->GetSize();    
    //Texture* texture = Texture::Add(textureFile);
    //Vector2 size = material->GetDiffuseMap()->GetSize();
    //localScale = Vector3(size.x, size.y);

    size = material->GetDiffuseMap()->GetSize();

    vector<VertexUV>& vertices = mesh->GetVertices();
    vertices[0] = VertexUV(-0.5f * size.x, +0.5f * size.y, 0.0f, 0, 0);//0
    vertices[1] = VertexUV(+0.5f * size.x, +0.5f * size.y, 0.0f, 1, 0);//0
    vertices[2] = VertexUV(-0.5f * size.x, -0.5f * size.y, 0.0f, 0, 1);//0
    vertices[3] = VertexUV(+0.5f * size.x, -0.5f * size.y, 0.0f, 1, 1);//0    

    mesh->UpdateVertices();
}