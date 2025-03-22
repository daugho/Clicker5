#pragma once
class Shop;
class ClickerUIManager : public Singleton<ClickerUIManager>
{
    friend class Singleton;
public:
    ClickerUIManager();
    ~ClickerUIManager();

    void Update();
    void Render();
    void Edit();
    void ToggleInventory();
    void ToogleFreeView();
    //void ShowMiningEffect();

    void SetCursorColor(const Float4& color);
    void ResetCursorColor();

    void OpenShopUI(int shopID);
    void CloseShopUI();

    void SetCurrentShop(Shop* shop);
    OreInventory* GetInventory() { return inventory; }
    bool IsInventoryOpen() const { return isInventoryOpen;}
    bool IsShopOpen() const { return isShopOpen;}
    int GetCurrentShopID() const { return currentShopID; }
private:
    void CreateUI();
    void HandleUIToggle();

private:
    bool isInventoryOpen = false;
    bool isShopOpen = false;

    int currentShopID = -1 ;

    Quad* cursor;
    Quad* miningEffect;
    Quad* resourcePanel;

    OreInventory* inventory;
    Shop* shopOpen;
    BlendState* blendState[2];

    Float4 originalCursorColor = Float4(1, 1, 1, 1);

    bool isFreeView = false;
};