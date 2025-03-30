#pragma once
class BoxInventory;

class Boxmanager : public Singleton<Boxmanager>
{
public:
    Boxmanager();
    ~Boxmanager();

    void Update();
    void Render();
    void Edit();
    void AddBox(Vector3 pos);
    BoxInventory* GetClosestBox(const Ray& ray, RaycastHit* hit);

    vector<BoxInventory*>& GetBoxes() { return boxes; }
private:
    BoxInventory* box;
    vector<BoxInventory*> boxes;
};