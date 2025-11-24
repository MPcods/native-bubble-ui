#include "animation.h"
#include "math_utils.h"

FloatAnimation::FloatAnimation(float startValue)
        : current(startValue),
          start(startValue),
          target(startValue),
          startTime(0.0),
          duration(0.0),
          animating(false) {}

void FloatAnimation::setTarget(float t, float durationSec) {
    start = current;
    target = t;
    duration = durationSec;
    startTime = nowSeconds();
    animating = durationSec > 0.0;
    if (!animating) {
        current = target;
    }
}

void FloatAnimation::update(double nowSec) {
    if (!animating) return;
    double elapsed = nowSec - startTime;
    if (elapsed >= duration) {
        current = target;
        animating = false;
        return;
    }
    float t = (float)(elapsed / duration);
    current = lerp(start, target, t);
}