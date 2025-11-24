#pragma once
#include "ui_element.h"
#include "animation.h"

class Renderer; // forward

class BubbleButton : public UIElement {
public:
    BubbleButton(Renderer* renderer);

    void setPosition(float x, float y);
    void setRadius(float r);
    void setOnToggleMenu(bool* menuOpenFlag, FloatAnimation* animOpenAmount);

    virtual void draw() override;
    virtual bool handleTouch(const TouchEvent& ev) override;

    float getX() const { return cx; }
    float getY() const { return cy; }
    float getRadius() const { return radius; }

private:
    Renderer* renderer;
    float cx = 0.0f;
    float cy = 0.0f;
    float radius = 80.0f;

    bool* menuOpen = nullptr;
    FloatAnimation* openAnim = nullptr;
};