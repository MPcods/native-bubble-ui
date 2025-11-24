#include "menu_layout.h"
#include "bubble_button.h"
#include "renderer.h"

MenuLayout::MenuLayout(Renderer* r)
        : renderer(r) {
    // Drei Beispiel-Buttons
    buttons.emplace_back(std::make_unique<MenuButton>(renderer, "Action 1"));
    buttons.back()->setOnClickId(1);

    buttons.emplace_back(std::make_unique<MenuButton>(renderer, "Action 2"));
    buttons.back()->setOnClickId(2);

    buttons.emplace_back(std::make_unique<MenuButton>(renderer, "Action 3"));
    buttons.back()->setOnClickId(3);
}

void MenuLayout::attachToBubble(BubbleButton* bubble, FloatAnimation* openAnim) {
    this->bubble = bubble;
    this->openAnim = openAnim;
}

void MenuLayout::updateLayout() {
    if (!bubble || !openAnim) return;
    float t = openAnim->getValue(); // 0..1
    if (t <= 0.001f) return;

    float bx = bubble->getX();
    float by = bubble->getY();

    float baseOffset = 120.0f * t;
    float spacing = 140.0f * t;

    for (size_t i = 0; i < buttons.size(); ++i) {
        float y = by - baseOffset - spacing * (float)i;
        float x = bx - 140.0f * t;
        buttons[i]->setPosition(x, y);
        buttons[i]->setRadius(60.0f * (0.7f + 0.3f * t));
    }
}

bool MenuLayout::handleTouch(const TouchEvent& ev) {
    if (!openAnim || openAnim->getValue() <= 0.01f)
        return false;

    updateLayout();

    for (auto& b : buttons) {
        if (b->handleTouch(ev)) return true;
    }
    return false;
}

void MenuLayout::draw() {
    if (!openAnim || openAnim->getValue() <= 0.01f)
        return;

    updateLayout();

    for (auto& b : buttons) {
        b->draw();
    }
}