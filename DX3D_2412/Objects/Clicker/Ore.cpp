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

    collider->SetParent(this);
    collider->SetLocalPosition(Vector3(0, 0, 0));
    collider->SetLocalScale(Vector3(1,1,1));
    particle = new ParticleSystem("Resources/FX/Explosion.fx");
    particle->SetLoop(false);
    particle->Stop();

	hpBar = new ProgressBar(L"Resources/Textures/UI/enemy_hp_bar.png", L"Resources/Textures/UI/enemy_hp_bar_BG.png");
}

Ore::~Ore()
{
    delete particle;
    delete collider;
    delete model;
}

void Ore::Render()
{
    model->Render();
    if (particle)
        particle->Render();
    collider->Render();
}

void Ore::PostRender()
{
    Vector3 pos = CAM->WorldToScreenPoint(localPosition + hpBarOffset);
    Font::Get()->RenderText(data.name, pos);

    hpBar->SetLocalPosition(pos);
    hpBar->UpdateWorld();
    hpBar->Render();
}

void Ore::Update()
{
    UpdateWorld();
    collider->UpdateWorld();
    model->UpdateWorld();
    if (particle)
        particle->Update();
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


    for (DropData& drop : dropList)
    {
        cumulativeRate += drop.dropRate;
        if (randValue <= cumulativeRate)
        {
            selectedItem = &drop;
            break;
        }
    }
    uniform_int_distribution<int> countDistribution(selectedItem->minCount, selectedItem->maxCount);
    int itemCount = countDistribution(generator);


    if (inventory->GetTotalItemCount() >= inventory->GetMaxCapacity())
        return;

    inventory->AddItem(*selectedItem, itemCount);

}

void Ore::DropItemsToHelper()
{

    HelperInventory* inventory = ClickerUIManager::Get()->GetHelperInventory();
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

    inventory->AddItem(*selectedItem, itemCount);
}

void Ore::TakeDamage(int damage)
{
    health -= damage;
    hpBar->SetAmount((float)health / data.health);
    if (particle && !particle->IsActive())
        particle->Play(GetGlobalPosition());

    if (health <= 0)
    {
        DropItems();
        health = data.health;
    }
}

void Ore::TakeDamageFroHelper(int damage)
{
    health -= damage;

    if (particle && !particle->IsActive())
        particle->Play(GetGlobalPosition());

    if (health <= 0)
    {
        DropItemsToHelper();
        health = data.health;
    }
}

bool Ore::IsRayCollision(const Ray& ray, RaycastHit* hit)
{
    return collider->IsRayCollision(ray, hit);
}

void Ore::SetColliderColor(const Float4& color)
{
    collider->GetMaterial()->GetData()->diffuse = color;
}

void Ore::SetData(const oreData& data)
{
    this->data = data;
    this->health = data.health;
}
