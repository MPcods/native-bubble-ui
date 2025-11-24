#pragma once
#include <GLES2/gl2.h>

class ShaderProgram {
public:
    ShaderProgram();
    ~ShaderProgram();

    bool create(const char* vsSource, const char* fsSource);
    void destroy();

    void use() const;
    GLint getAttribLocation(const char* name) const;
    GLint getUniformLocation(const char* name) const;

private:
    GLuint program = 0;
    GLuint compileShader(GLenum type, const char* source);
};