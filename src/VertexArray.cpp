#include "VertexArray.hpp"
#include "Renderer.hpp"
#include "VertexBufLayout.hpp"
#include "VertexBuffer.hpp"
#include <cstddef>
#include <cstdint>

VertexArray::VertexArray() {
    GL_CALL(glGenVertexArrays(1, &rendererId));
}
VertexArray::~VertexArray() { GL_CALL(glDeleteVertexArrays(1, &rendererId)); }

void VertexArray::bind() const {
    GL_CALL(glBindVertexArray(rendererId));
}
void VertexArray::unbind() const {
    GL_CALL(glBindVertexArray(0));
}

void VertexArray::addBuffer(const VertexBuffer& vb,
                            const VertexBufLayout& layout) {
    bind();
    vb.bind();
    const auto& elements = layout.getElements();
    std::uintptr_t offset = 0;
    for (std::size_t i = 0; i < elements.size(); i++) {
        const auto& element = elements[i];
        GL_CALL(glEnableVertexAttribArray(i));
        GL_CALL(glVertexAttribPointer(i, element.count, element.type,
                                      element.normalized, layout.getStride(),
                                      (void*)offset));
        
        offset += element.count * VertexBufElement::sizeOfType(element.type);
    }
}
