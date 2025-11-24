#include "shader_program.h"
#include <android/log.h>

#define LOG_TAG "NUI_SHADER"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

ShaderProgram::ShaderProgram() = default;

ShaderProgram::~ShaderProgram() {
    destroy();
}

GLuint ShaderProgram::compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    GLint compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint len = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        if (len > 0) {
            char* buf = new char[len];
            glGetShaderInfoLog(shader, len, nullptr, buf);
            LOGE("Shader compile error: %s", buf);
            delete[] buf;
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

bool ShaderProgram::create(const char* vsSource, const char* fsSource) {
    GLuint vs = compileShader(GL_VERTEX_SHADER, vsSource);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fsSource);
    if (!vs || !fs) {
        if (vs) glDeleteShader(vs);
        if (fs) glDeleteShader(fs);
        return false;
    }

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    GLint linked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint len = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
        if (len > 0) {
            char* buf = new char[len];
            glGetProgramInfoLog(program, len, nullptr, buf);
            LOGE("Program link error: %s", buf);
            delete[] buf;
        }
        glDeleteProgram(program);
        program = 0;
        return false;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
    return true;
}

void ShaderProgram::destroy() {
    if (program) {
        glDeleteProgram(program);
        program = 0;
    }
}

void ShaderProgram::use() const {
    glUseProgram(program);
}

GLint ShaderProgram::getAttribLocation(const char* name) const {
    return glGetAttribLocation(program, name);
}

GLint ShaderProgram::getUniformLocation(const char* name) const {
    return glGetUniformLocation(program, name);
}