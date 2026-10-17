#include "debugmacros.hpp"
#include "glad/gl.h"
#include "shader-parser.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <unordered_map>
#include <vector>

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

// Deals with GLFW errors
static void glfwErrCallback(int err_code, const char* err_desc);
// Viewports the window when resized
static void glfwResizeCallback(GLFWwindow* window, int width, int height);
// Shader garbage
static GLuint compileShader(GLuint shaderType, const char* source);
static GLuint createShader(const char* vertexShaderSrc,
                           const char* fragmentShaderSrc);

int main() {
    glfwSetErrorCallback(glfwErrCallback);
    if (!glfwInit()) {
        const char* description;
        int code{glfwGetError(&description)};
        printf("GLFW Initialization failed: %s (Code: %d)\n", description,
               code);
        return 1;
    }

    // MODERNIZED profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__ // Apple lovers
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow* window;
    if (!(window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Cool Window",
                                    nullptr, nullptr))) {
        // Assign and check condition in one step (1000 IQ)
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }
    // Apple lovers with their retina displays
    int frameBufferWidth, frameBufferHeight;
    glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
    // Maps (-1, 1) to the screen size
    glViewport(0, 0, frameBufferWidth, frameBufferHeight);
    glfwSetFramebufferSizeCallback(window, glfwResizeCallback);
    // clang-format off
    GLfloat vertices[]{
	-0.5f, -0.5f, 
       	 0.5f, -0.5f,
	-0.5f,  0.5f,
         0.5f,  0.5f,
    };
    GLuint indices[]{
	0, 1, 2,
	1, 2, 3,
    };
    // clang-format on
    GLuint vertexBuffer, vertexArray, indexBuffer;
    GL_CALL(glGenVertexArrays(1, &vertexArray));
    GL_CALL(glGenBuffers(1, &vertexBuffer));
    GL_CALL(glGenBuffers(1, &indexBuffer));
    GL_CALL(glBindVertexArray(vertexArray));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
    /* GL_STREAM_DRAW: the data is set only once and used by the GPU at most a
       few times. GL_STATIC_DRAW: the data is set only once and used many times.
       GL_DYNAMIC_DRAW: the data is changed a lot and used many times. */
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                         GL_STATIC_DRAW));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                         GL_STATIC_DRAW));
    GL_CALL(glEnableVertexAttribArray(0));
    GL_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
                                  nullptr));
    std::unordered_map shaderMap{parseShader("shaders.glsl").value()};
    GLuint program = createShader(shaderMap.at("vertex").c_str(),
                                  shaderMap.at("fragment").c_str());
    GL_CALL(glUseProgram(program));
    GLint uniform = glGetUniformLocation(program, "u_Color");
    RELASSERT(uniform != -1);
    GLfloat rgb[3] = {1.0f, 0.f, 0.f};
    int chosen = 0;
    while (!glfwWindowShouldClose(window)) {
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
        constexpr float speed = 0.05f;
        int colorIdx;
        if (rgb[(colorIdx = (chosen + 2) % 3)] > 0.f) {
            rgb[colorIdx] -= speed; // Decrease previous
        } else if (rgb[(colorIdx = (chosen + 1) % 3)] < 1.0f) {
	    rgb[colorIdx] += speed; // Increase next
        } else {
	    chosen = (chosen + 1) % 3; // Advance
        }
	GL_CALL(glUniform4f(uniform, rgb[0], rgb[1], rgb[2], 1.0f));
        GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

static void glfwErrCallback(int err_code, const char* err_desc) {
    std::cerr << "GLFW Error (" << err_code << "): " << err_desc << '\n';
}
static void glfwResizeCallback([[maybe_unused]] GLFWwindow* window, int width,
                               int height) {
    glViewport(0, 0, width, height);
}

static GLuint compileShader(GLenum shaderType, const char* source) {
    GLuint shaderId{glCreateShader(shaderType)};
    glShaderSource(shaderId, 1, &source, nullptr);
    glCompileShader(shaderId);
    GLint result;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    if (!result) {
        GLint logLength;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> infoLog(logLength);
        glGetShaderInfoLog(shaderId, logLength, &logLength, infoLog.data());
        std::cerr << "You little "
                  << (shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment")
                  << "! WHY?: " << infoLog.data() << '\n';
        glDeleteShader(shaderId);
        return 0;
    }
    return shaderId;
}
static GLuint createShader(const char* vertexShaderSrc,
                           const char* fragmentShaderSrc) {
    GL_CALL(GLuint program{glCreateProgram()});
    GLuint vertexShader{compileShader(GL_VERTEX_SHADER, vertexShaderSrc)};
    GLuint fragmentShader{compileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc)};
    RELASSERT(program);
    RELASSERT(vertexShader);
    RELASSERT(fragmentShader);
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    // glValidateProgram(program); "unnecessary overhead"
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}
