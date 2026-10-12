#include "glad/gl.h"
#include "shader-parser.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

#ifdef NDEBUG // C++ commitee fail
#undef NDEBUG
#include <cassert>
#define NDEBUG
#else
#include <cassert>
#endif

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

// Deals with errors
static void glfwErrCallback(int err_code, const char* err_desc);
// Viewports the window when resized
static void glfwResizeCallback(GLFWwindow* window, int width, int height);
// Shader garbage
static unsigned int compileShader(unsigned int shaderType, std::string_view);
static unsigned int createShader(std::string_view vertexShaderSrc,
                                 std::string_view fragmentShaderSrc);

int main() {
    glfwSetErrorCallback(glfwErrCallback);
    if (!glfwInit()) {
        const char* description;
        int code = glfwGetError(&description);
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
    
    std::unordered_map shaderMap = parseShader("shaders.glsl").value();
    
    const std::string vertexShaderCode = shaderMap.at("vertex");
    const std::string fragmentShaderCode = shaderMap.at("fragment");
    unsigned int shaderProgram =
        createShader(vertexShaderCode, fragmentShaderCode);

    float vertices[] = {-0.5f, -0.5f, 0.5f, -0.5f, 0.0f, 0.5f};
    unsigned int vertexBuffer, vertexArray;
    glGenVertexArrays(1, &vertexArray);
    glGenBuffers(1, &vertexBuffer);
    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    /* GL_STREAM_DRAW: the data is set only once and used by the GPU at most a
       few times. GL_STATIC_DRAW: the data is set only once and used many times.
       GL_DYNAMIC_DRAW: the data is changed a lot and used many times. */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(shaderProgram);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

static void glfwErrCallback(int err_code, const char* err_desc) {
    std::cerr << "GLFW Error (" << err_code << "): " << err_desc << '\n';
}
static void glfwResizeCallback([[maybe_unused]] GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
static unsigned int compileShader(unsigned int shaderType,
                                  std::string_view stringViewSource) {
    const char* source = stringViewSource.data();
    unsigned int shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, &source, nullptr);
    glCompileShader(shaderId);
    int result;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    if (!result) {
        int logLength;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> infoLog(logLength);
        glGetShaderInfoLog(shaderId, logLength, &logLength, infoLog.data());
        std::cerr << "You "
                  << (shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment")
                  << "! Why: " << infoLog.data() << '\n';
        glDeleteShader(shaderId);
        return 0;
    }
    return shaderId;
}
static unsigned int createShader(std::string_view vertexShaderSrc,
                                 std::string_view fragmentShaderSrc) {
    unsigned int program = glCreateProgram();
    unsigned int vertexShader =
        compileShader(GL_VERTEX_SHADER, vertexShaderSrc);
    unsigned int fragmentShader =
        compileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);
    assert(vertexShader);
    assert(fragmentShader);
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glValidateProgram(program);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}
