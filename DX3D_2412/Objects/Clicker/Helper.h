#pragma once

class Helper : public SphereCollider
{
public:
    enum class State
    {
        Idle,
        MovingToOre,
        MovingToBox,
        ManualMove
    };
    Helper();
    ~Helper();

    void Update();
    void Render();
    void Edit();

    void SetCube(Cube* cube) { this->cube = cube; }
    void SetAStar(AStar* astar) { this->aStar = astar; }
    void SetTerrain(Terrain* terrain) { this->terrain = terrain; }
    void MoveAlongPath();
    void FindOre();
    void SetManualPath(const vector<Vector3>& newPath);
    State GetState() {
        return currentState;
    }

private:
    void Control();
    void Move();
    void Rotate();


private:

    State currentState = State::Idle;
    Terrain* terrain;
    ModelAnimator* model;
    float debugTimer = 0.0f;
    float moveSpeed = 5.0f;
    float rotSpeed = 10.0f;

    Vector3 velocity;
    Vector3 destPos;
    vector<Vector3> path;
    int pathIndex = 0;

    AStar* aStar = nullptr;
    Cube* cube = nullptr;

    Ore* targetOre = nullptr;
    BoxInventory* targetBox = nullptr;
    int currentIndex = 0;

    Vector3 targetPos;
};