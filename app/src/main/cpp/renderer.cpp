#include "renderer.h"
#include "time_utils.h"
#include <android/log.h>
#include <math.h>

#define LOG_TAG_REND "NUI_REND"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG_REND, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG_REND, __VA_ARGS__)

Renderer::Renderer() : menuOpenAnim(0.0f) {}

Renderer::~Renderer() {
    destroy();
}

bool Renderer::init(ANativeWindow* win) {
    window = win;
    screenW = ANativeWindow_getWidth(window);
    screenH = ANativeWindow_getHeight(window);

    if (!initEGL()) {
        LOGE("initEGL failed");
        return false;
    }

    initGLObjects();

    initialized = true;
    lastTime = nowSeconds();
    return true;
}

void Renderer::destroy() {
    uiElements.clear();
    bubble.reset();
    menuLayout.reset();
    destroyEGL();
    initialized = false;
}

bool Renderer::initEGL() {
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY) {
        LOGE("No EGL display");
        return false;
    }
    if (!eglInitialize(display, nullptr, nullptr)) {
        LOGE("eglInitialize failed");
        return false;
    }

    EGLint configAttribs[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_DEPTH_SIZE, 0,
            EGL_NONE
    };

    EGLConfig config;
    EGLint numConfigs;
    eglChooseConfig(display, configAttribs, &config, 1, &numConfigs);

    EGLint format;
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
    ANativeWindow_setBuffersGeometry(window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, window, nullptr);
    if (surface == EGL_NO_SURFACE) {
        LOGE("eglCreateWindowSurface failed");
        return false;
    }

    EGLint ctxAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, ctxAttribs);
    if (context == EGL_NO_CONTEXT) {
        LOGE("eglCreateContext failed");
        return false;
    }

    if (!eglMakeCurrent(display, surface, surface, context)) {
        LOGE("eglMakeCurrent failed");
        return false;
    }

    return true;
}

void Renderer::destroyEGL() {
    if (display != EGL_NO_DISPLAY) {
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (context != EGL_NO_CONTEXT) eglDestroyContext(display, context);
        if (surface != EGL_NO_SURFACE) eglDestroySurface(display, surface);
        eglTerminate(display);
    }
    display = EGL_NO_DISPLAY;
    context = EGL_NO_CONTEXT;
    surface = EGL_NO_SURFACE;
}

void Renderer::resize(int w, int h) {
    screenW = w;
    screenH = h;
    glViewport(0, 0, screenW, screenH);
}

void Renderer::initGLObjects() {
    const char* vsSrc = R"(
        attribute vec2 aPosition;
        uniform vec2 uResolution;
        void main() {
            // Pixel-Koordinaten in NDC
            vec2 zeroToOne = aPosition / uResolution;
            vec2 zeroToTwo = zeroToOne * 2.0;
            vec2 clip = zeroToTwo - 1.0;
            gl_Position = vec4(clip * vec2(1, -1), 0.0, 1.0);
        }
    )";

    const char* fsSrc = R"(
        precision mediump float;
        uniform vec4 uColor;
        void main() {
            gl_FragColor = uColor;
        }
    )";

    if (!colorProgram.create(vsSrc, fsSrc)) {
        LOGE("Failed to create colorProgram");
    }
    colorProgram.use();
    attrPos = colorProgram.getAttribLocation("aPosition");
    uniColor = colorProgram.getUniformLocation("uColor");
    uniResolution = colorProgram.getUniformLocation("uResolution");
    glUniform2f(uniResolution, (float)screenW, (float)screenH);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // Bubble + Menü anlegen
    bubble = std::make_unique<BubbleButton>(this);
    float bubbleRadius = 80.0f;
    float bx = (float)screenW - bubbleRadius - 40.0f;
    float by = (float)screenH - bubbleRadius - 40.0f;
    bubble->setPosition(bx, by);
    bubble->setRadius(bubbleRadius);
    bubble->setOnToggleMenu(&menuOpen, &menuOpenAnim);

    menuLayout = std::make_unique<MenuLayout>(this);
    menuLayout->attachToBubble(bubble.get(), &menuOpenAnim);

    uiElements.emplace_back(std::move(bubble));
    uiElements.emplace_back(std::move(menuLayout));

    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
}

void Renderer::update(double nowSec) {
    menuOpenAnim.update(nowSec);
}

void Renderer::renderFrame() {
    if (!initialized) return;

    double now = nowSeconds();
    update(now);
    lastTime = now;

    glClear(GL_COLOR_BUFFER_BIT);

    colorProgram.use();
    glUniform2f(uniResolution, (float)screenW, (float)screenH);

    for (auto& e : uiElements) {
        e->draw();
    }

    eglSwapBuffers(display, surface);
}

void Renderer::onTouch(float x, float y, int action) {
    TouchEvent ev{};
    ev.x = x;
    ev.y = y;
    ev.action = action;

    for (auto it = uiElements.rbegin(); it != uiElements.rend(); ++it) {
        if ((*it)->handleTouch(ev))
            break;
    }
}

void Renderer::drawCircle(float cx, float cy, float radius,
                          float rCol, float gCol, float bCol, float aCol) {
    const int segments = 48;
    float verts[(segments + 2) * 2];

    verts[0] = cx;
    verts[1] = cy;

    for (int i = 0; i <= segments; ++i) {
        float theta = (float)i / (float)segments * 2.0f * (float)M_PI;
        verts[(i+1)*2 + 0] = cx + cosf(theta) * radius;
        verts[(i+1)*2 + 1] = cy + sinf(theta) * radius;
    }

    glEnableVertexAttribArray(attrPos);
    glVertexAttribPointer(attrPos, 2, GL_FLOAT, GL_FALSE, 0, verts);
    glUniform4f(uniColor, rCol, gCol, bCol, aCol);
    glDrawArrays(GL_TRIANGLE_FAN, 0, segments + 2);
    glDisableVertexAttribArray(attrPos);
}