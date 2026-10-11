#include "glad/gl.h"
#include <GLFW/glfw3.h>
#include <iostream>

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

// Deals with errors
void glfw_err_callback(int err_code, const char* err_desc);
// Viewports the window when resized
void glfw_resize_callback(GLFWwindow* window, int width, int height);

int main() {
    glfwSetErrorCallback(glfw_err_callback);
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
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // Maps (-1, 1) to the screen size
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glfwSetFramebufferSizeCallback(window, glfw_resize_callback);

    const char* vertex_shader_code =
        R"(#version 330 core
layout (location = 0) in vec3 aPos;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
})";
    const char* frag_shader_code = R"(#version 330 core
out vec4 FragColor;

void main() {
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
})";
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_code, nullptr);
    glCompileShader(vertex_shader);
    glShaderSource(fragment_shader, 1, &frag_shader_code, nullptr);
    glCompileShader(fragment_shader);
    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f,
                        0.0f,  0.0f,  0.5f, 0.0f};
    unsigned int vertex_buf, vertex_arr;
    glGenVertexArrays(1, &vertex_arr);
    glGenBuffers(1, &vertex_buf);
    glBindVertexArray(vertex_arr);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buf);
    /* GL_STREAM_DRAW: the data is set only once and used by the GPU at most a
       few times. GL_STATIC_DRAW: the data is set only once and used many times.
       GL_DYNAMIC_DRAW: the data is changed a lot and used many times. */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader_program);
        glBindVertexArray(vertex_arr);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void glfw_err_callback(int err_code, const char* err_desc) {
    std::cerr << "GLFW Error (" << err_code << "): " << err_desc << '\n';
}
void glfw_resize_callback(GLFWwindow* window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height);
}
