#include "Renderer.hpp"
#include "IndexBuffer.hpp"
#include <iostream>

IndexBuffer::IndexBuffer(const GLuint* data, GLuint count) : vertex_count{count} {
    GL_CALL(glGenBuffers(1, &id));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
    /* GL_STREAM_DRAW: the data is set only once and used by the GPU at most a
       few times. GL_STATIC_DRAW: the data is set only once and used many times.
       GL_DYNAMIC_DRAW: the data is changed a lot and used many times. */    
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW));
}
IndexBuffer::~IndexBuffer() {
    GL_CALL(glDeleteBuffers(1, &id));
}

void IndexBuffer::bind() const {
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
}
void IndexBuffer::unbind() const {
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
