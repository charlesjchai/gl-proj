#pragma once
#include "glad/gl.h"
#include <cstddef>

class VertexBuffer {
    GLuint id;

  public:
    VertexBuffer(const void* data, std::size_t size);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;
};
