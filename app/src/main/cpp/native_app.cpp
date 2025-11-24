#include "native_app.h"
#include <android/log.h>

#define LOG_TAG_APP "NUI_APP"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG_APP, __VA_ARGS__)

NativeApp::NativeApp(android_app* a)
        : app(a) {}

void NativeApp::handleCmdProxy(android_app* app, int32_t cmd) {
    auto* na = reinterpret_cast<NativeApp*>(app->userData);
    if (na) na->handleCmd(cmd);
}

int32_t NativeApp::handleInputProxy(android_app* app, AInputEvent* event) {
    auto* na = reinterpret_cast<NativeApp*>(app->userData);
    return na ? na->handleInput(event) : 0;
}

void NativeApp::handleCmd(int32_t cmd) {
    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            if (app->window) {
                LOGI("APP_CMD_INIT_WINDOW");
                renderer.init(app->window);
                hasWindow = true;
            }
            break;
        case APP_CMD_TERM_WINDOW:
            LOGI("APP_CMD_TERM_WINDOW");
            renderer.destroy();
            hasWindow = false;
            break;
        default:
            break;
    }
}

int32_t NativeApp::handleInput(AInputEvent* event) {
    if (AInputEvent_getType(event) != AINPUT_EVENT_TYPE_MOTION)
        return 0;

    int32_t action = AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK;
    float x = AMotionEvent_getX(event, 0);
    float y = AMotionEvent_getY(event, 0);

    // mappe auf 0(DOWN),1(UP),2(MOVE)
    int mappedAction = 2;
    if (action == AMOTION_EVENT_ACTION_DOWN) mappedAction = 0;
    else if (action == AMOTION_EVENT_ACTION_UP) mappedAction = 1;
    else if (action == AMOTION_EVENT_ACTION_MOVE) mappedAction = 2;

    renderer.onTouch(x, y, mappedAction);
    return 1;
}

void NativeApp::run() {
    app->userData = this;
    app->onAppCmd = &NativeApp::handleCmdProxy;
    app->onInputEvent = &NativeApp::handleInputProxy;

    while (true) {
        int events;
        android_poll_source* source;

        while (ALooper_pollAll(0, nullptr, &events, (void**)&source) >= 0) {
            if (source) source->process(app, source);

            if (app->destroyRequested) {
                renderer.destroy();
                return;
            }
        }

        if (hasWindow) {
            renderer.renderFrame();
        }
    }
}