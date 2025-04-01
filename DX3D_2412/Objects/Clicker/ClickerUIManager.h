#pragma once
class Shop;

class Shop;
class OreInventory;
class Quad;
class BlendState;
class GoldDisplay;
class ItemPopup;
class BoxInventory;
class BoxUI;
class HelperInventory;
enum class UIState
{
    None,
    Inventory,
    Shop,
    Dialogue,
    Upgrade,
    Box
};

class ClickerUIManager : public Singleton<ClickerUIManager>
{
    friend class Singleton;

public:
    ClickerUIManager();
    ~ClickerUIManager();

    void Update();
    void Render();
    void Edit();

    void OpenInventory();
    void OpenShopUI(int shopID);
    void CloseCurrentUI();
    void OpenBoxUI();
    void SetCursorColor(const Float4& color);
    void ResetCursorColor();

    OreInventory* GetInventory() { return inventory; }
    HelperInventory* GetHelperInventory() { return helperinventory; }
    ItemPopup* GetItemPopup() { return itemPopup; }
    bool IsInventoryOpen() const { return currentUIState == UIState::Inventory; }
    bool IsShopOpen() const { return currentUIState == UIState::Shop; }
    bool IsBoxOPen() const{return currentUIState== UIState::Box;}
    int GetCurrentShopID() const { return currentShopID; }
    BoxUI* GetBoxUI() { return boxUI; }

private:
    void CreateUI();
    void HandleUIInput();
    void UpdateCurrentUI();
    void ToogleFreeView();

private:
    UIState currentUIState = UIState::None;

    int currentShopID = -1;

    Quad* cursor;
    Quad* miningEffect;
    Quad* resourcePanel;

    OreInventory* inventory;
    HelperInventory* helperinventory;
    Shop* shopOpen;//shop UI

    BoxInventory* boxinventory;
    BoxUI* boxUI;

    BlendState* blendState[2];

    Float4 originalCursorColor = Float4(1, 1, 1, 1);
    bool isFreeView = false;
    GoldDisplay* goldDisplay;
    ItemPopup* itemPopup;
};
