#pragma once
#include "time_utils.h"

class FloatAnimation {
public:
    FloatAnimation(float startValue = 0.0f);

    void setTarget(float target, float durationSec);
    void update(double nowSec);
    float getValue() const { return current; }
    bool isAnimating() const { return animating; }

private:
    float current;
    float start;
    float target;
    double startTime;
    double duration;
    bool animating;
};