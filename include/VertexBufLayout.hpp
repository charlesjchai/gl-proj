#pragma once
#include "glad/gl.h"
#include "Renderer.hpp"
#include <vector>

struct VertexBufElement {
    GLuint count;
    GLuint type;
    GLboolean normalized;

    static GLuint sizeOfType(GLenum type) {
        switch (type) {
        case GL_FLOAT:
            return 4;
        case GL_UNSIGNED_INT:
            return 4;
        case GL_UNSIGNED_BYTE:
            return 1;
        }
        RELASSERT("Invalid type" && false);
        return 0;
    }
};
class VertexBufLayout {
    std::vector<VertexBufElement> elements;
    GLuint stride;
  public:
    VertexBufLayout() : stride{0} {}
    template <typename T> void push(GLuint, GLboolean) {
	static_assert(false, "Invalid type");
    }
    template <> void push<GLfloat>(GLuint count, GLboolean normalized) {
        elements.push_back({count, GL_FLOAT, normalized});
        stride += count * 4;
    }
    template <> void push<GLuint>(GLuint count, GLboolean normalized) {
        elements.push_back({count, GL_UNSIGNED_INT, normalized});
        stride += count * 4;
    }
    template <> void push<GLubyte>(GLuint count, GLboolean normalized) {
        elements.push_back({count, GL_UNSIGNED_BYTE, normalized});
        stride += count * 1;        
    }

    const std::vector<VertexBufElement>& getElements() const { return elements; }
    GLuint getStride() const { return stride; }
};
