#include "bubble_button.h"
#include "renderer.h"
#include <math.h>
#include <android/log.h>

#define LOG_TAG_BUBBLE "NUI_BUBBLE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG_BUBBLE, __VA_ARGS__)

BubbleButton::BubbleButton(Renderer* r)
        : renderer(r) {}

void BubbleButton::setPosition(float x, float y) {
    cx = x;
    cy = y;
}

void BubbleButton::setRadius(float r) {
    radius = r;
}

void BubbleButton::setOnToggleMenu(bool* menuOpenFlag, FloatAnimation* animOpenAmount) {
    menuOpen = menuOpenFlag;
    openAnim = animOpenAmount;
}

bool BubbleButton::handleTouch(const TouchEvent& ev) {
    float dx = ev.x - cx;
    float dy = ev.y - cy;
    float dist = sqrtf(dx*dx + dy*dy);

    if (dist <= radius && ev.action == 0) { // DOWN
        if (menuOpen && openAnim) {
            *menuOpen = !*menuOpen;
            float target = *menuOpen ? 1.0f : 0.0f;
            openAnim->setTarget(target, 0.25f); // 250ms Animation
            LOGI("Bubble touched -> menuOpen=%d", *menuOpen ? 1 : 0);
        }
        return true;
    }
    return false;
}

void BubbleButton::draw() {
    if (!renderer) return;
    // zeichne gefüllten Kreis (blau)
    renderer->drawCircle(cx, cy, radius, 0.2f, 0.6f, 1.0f, 0.9f);
}