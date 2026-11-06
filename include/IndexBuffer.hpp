#pragma once
#include "glad/gl.h"
#include <cstddef>

class IndexBuffer {
    GLuint id;
    std::size_t vertex_count;
    bool destroyed;
  public:
    IndexBuffer(const GLuint* data, GLuint count);
    ~IndexBuffer();

    void destroy();
    void bind() const;
    void unbind() const;
    GLuint getCount() const { return vertex_count; }
};

