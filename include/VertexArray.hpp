#pragma once
#include "VertexBuffer.hpp"
#include "VertexBufLayout.hpp"

class VertexArray {
    GLuint rendererId;
  public:
    VertexArray();
    ~VertexArray();

    void bind() const;
    void unbind() const;
    void addBuffer(const VertexBuffer& vb, const VertexBufLayout& layout);
};
