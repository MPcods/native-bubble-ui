#include "menu_button.h"
#include "renderer.h"
#include <math.h>
#include <android/log.h>

#define LOG_TAG_MBTN "NUI_MBTN"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG_MBTN, __VA_ARGS__)

MenuButton::MenuButton(Renderer* r, const std::string& label)
        : renderer(r), label(label) {}

void MenuButton::setPosition(float x, float y) {
    cx = x;
    cy = y;
}

void MenuButton::setRadius(float r) {
    radius = r;
}

bool MenuButton::handleTouch(const TouchEvent& ev) {
    float dx = ev.x - cx;
    float dy = ev.y - cy;
    float dist = sqrtf(dx*dx + dy*dy);
    if (dist <= radius && ev.action == 0) { // DOWN
        LOGI("Menu button clicked: %s (id=%d)", label.c_str(), onClickId);
        // hier könntest du JNI-Calls oder andere native Logik triggern
        return true;
    }
    return false;
}

void MenuButton::draw() {
    if (!renderer) return;
    // grauer Button
    renderer->drawCircle(cx, cy, radius, 0.3f, 0.3f, 0.3f, 0.9f);
    // Text könntest du später via Bitmap-Text oder externem Font-System machen
}