#include "Framework.h"

Ore::Ore(int oreID ,const string& modelPath) : oreID(oreID)
{
    data = OreManager::Get()->GetOreData(oreID);
    health = data.health;
    SetLocalScale(data.size);
    model = new Model(modelPath);
    model->SetParent(this);
    model->SetLocalPosition(0,0,0);
    model->SetLocalScale(3,3,3);

    if (!data.texturePath.empty())
    {
        for (Material* material : model->GetMaterials())
        {
            material->SetDiffuseMap(data.texturePath);
            material->SetSpecularMap(data.shaderPath);
			material ->SetNormalMap(data.normalPath);
        }
    }
    //hpBar = new ProgressBar(L"Resources/Textures/UI/enemy_hp_bar.png", L"Resources/Textures/UI/enemy_hp_bar_BG.png"); 택스처 적용법

    collider = new SphereCollider(3.0f);
    collider->SetParent(this);
    collider->SetLocalPosition(Vector3(0, 0, 0));
    collider->SetLocalScale(Vector3(0.25, 0.25, 0.25));
    //particle = new ParticleSystem("Resources/FX/Explosion.fx");

}

Ore::~Ore()
{
   // delete particle;
    delete collider;
    delete model;
}

void Ore::Render()
{
    model->Render();
    //particle->Render();
    //collider->Render();
}

void Ore::Update()
{
    UpdateWorld();
    collider->UpdateWorld();
    model->UpdateWorld();
    //if (!particle->IsActive())
    //    particle->Update();
}

void Ore::Edit()
{
    model->Edit();
    //collider->Edit();
}



void Ore::DropItems()
{

    OreInventory* inventory = ClickerUIManager::Get()->GetInventory();
    if (!inventory) return;

    vector<DropData>& dropList = OreManager::Get()->dropTable[oreID];
    if (dropList.empty()) return;

    static std::random_device rd;
    static std::mt19937 generator(rd());
    std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

    float randValue = distribution(generator);
    float cumulativeRate = 0.0f;
    DropData* selectedItem = nullptr;


    for (DropData& drop : dropList) {
        cumulativeRate += drop.dropRate;
        if (randValue <= cumulativeRate) {
            selectedItem = &drop;
            break;
        }
    }
    uniform_int_distribution<int> countDistribution(selectedItem->minCount, selectedItem->maxCount);
    int itemCount = countDistribution(generator);


    if (inventory->GetTotalItemCount() >= 10)
        return;

    inventory->AddItem(*selectedItem, itemCount);

}

void Ore::TakeDamage(int damage)
{
    health -= damage;

    if (health <= 0)
    {
        DropItems();
        ResetOre();
    }
}

void Ore::ResetOre()
{

}

bool Ore::IsRayCollision(const Ray& ray, RaycastHit* hit)
{
    return collider->IsRayCollision(ray, hit);
}

void Ore::SetColliderColor(const Float4& color)
{
    collider->GetMaterial()->GetData()->diffuse = color;
}
