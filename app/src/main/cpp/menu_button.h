#pragma once
#include "ui_element.h"
#include <string>

class Renderer;

class MenuButton : public UIElement {
public:
    MenuButton(Renderer* renderer, const std::string& label);

    void setPosition(float x, float y);
    void setRadius(float r);

    virtual void draw() override;
    virtual bool handleTouch(const TouchEvent& ev) override;

    void setOnClickId(int id) { onClickId = id; }

private:
    Renderer* renderer;
    std::string label;
    float cx = 0.0f;
    float cy = 0.0f;
    float radius = 60.0f;
    int onClickId = -1;
};