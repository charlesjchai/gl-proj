#pragma once
#include "glad/gl.h"
#include <cstddef>

class IndexBuffer {
    GLuint id;
    std::size_t vertex_count;

  public:
    IndexBuffer(const GLuint* data, GLuint count);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;
    GLuint getCount() const { return vertex_count; }
};

