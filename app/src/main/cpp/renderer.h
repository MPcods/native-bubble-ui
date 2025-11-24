#pragma once

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <vector>
#include <memory>
#include <android/native_window.h>
#include "shader_program.h"
#include "ui_element.h"
#include "bubble_button.h"
#include "menu_layout.h"
#include "animation.h"

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool init(ANativeWindow* window);
    void destroy();
    void resize(int w, int h);
    void renderFrame();

    void onTouch(float x, float y, int action);

    // vom Bubble/Menu benutzt:
    void drawCircle(float cx, float cy, float radius,
                    float rCol, float gCol, float bCol, float aCol);

private:
    ANativeWindow* window = nullptr;
    EGLDisplay display = EGL_NO_DISPLAY;
    EGLSurface surface = EGL_NO_SURFACE;
    EGLContext context = EGL_NO_CONTEXT;

    int screenW = 0;
    int screenH = 0;
    bool initialized = false;

    ShaderProgram colorProgram;
    GLint attrPos = -1;
    GLint uniColor = -1;
    GLint uniResolution = -1;

    std::vector<std::unique_ptr<UIElement>> uiElements;
    std::unique_ptr<BubbleButton> bubble;
    std::unique_ptr<MenuLayout> menuLayout;

    bool menuOpen = false;
    FloatAnimation menuOpenAnim;

    double lastTime = 0.0;

    bool initEGL();
    void destroyEGL();

    void initGLObjects();
    void update(double nowSec);
};