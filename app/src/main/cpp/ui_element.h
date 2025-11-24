#pragma once

struct TouchEvent {
    float x;
    float y;
    int action; // 0 = DOWN, 1 = UP, 2 = MOVE
};

class UIElement {
public:
    virtual ~UIElement() = default;
    virtual void draw() = 0;
    virtual bool handleTouch(const TouchEvent& ev) = 0;
};