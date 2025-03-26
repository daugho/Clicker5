#pragma once
class BoxInventory;

class BoxInventoryManager : public Singleton<BoxInventoryManager>
{
public:
    BoxInventoryManager();
    ~BoxInventoryManager();

    void Update();
    void Render();
    void Edit();
    void AddBox(Vector3 pos);
    BoxInventory* GetClosestBox(const Ray& ray, RaycastHit* hit);

    vector<BoxInventory*>& GetBoxes() { return boxes; }
private:
    vector<BoxInventory*> boxes;
};