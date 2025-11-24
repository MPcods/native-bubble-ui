#pragma once
#include <vector>
#include <memory>
#include "menu_button.h"
#include "animation.h"

class Renderer;
class BubbleButton;

class MenuLayout : public UIElement {
public:
    MenuLayout(Renderer* renderer);

    void attachToBubble(BubbleButton* bubble, FloatAnimation* openAnim);

    virtual void draw() override;
    virtual bool handleTouch(const TouchEvent& ev) override;

private:
    Renderer* renderer;
    BubbleButton* bubble = nullptr;
    FloatAnimation* openAnim = nullptr;
    std::vector<std::unique_ptr<MenuButton>> buttons;

    void updateLayout();
};
cpp
        Code kopieren
