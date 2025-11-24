#pragma once
#include <android_native_app_glue.h>
#include "renderer.h"

class NativeApp {
public:
    explicit NativeApp(android_app* app);

    void run();

    static void handleCmdProxy(android_app* app, int32_t cmd);
    static int32_t handleInputProxy(android_app* app, AInputEvent* event);

    void handleCmd(int32_t cmd);
    int32_t handleInput(AInputEvent* event);

private:
    android_app* app = nullptr;
    Renderer renderer;
    bool hasWindow = false;
};