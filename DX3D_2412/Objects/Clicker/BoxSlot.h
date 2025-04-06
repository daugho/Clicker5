#pragma once

class BoxSlot : public Button
{
public:
    BoxSlot();
    ~BoxSlot();

    void Update();
    void Render();
    void Edit();
    void SetImage(const wstring& texturePath,int index);
    void SetItem(const DropData& data,int count,int index);
    void ClearItem();
private:
    //Quad* image = nullptr;
    bool isVisible = false;
    DropData data;
    bool hasItem = false;
    int count = 0;
};