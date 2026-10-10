static_assert(sizeof(int) == 4, "Integer is not 4 bytes, bruh");
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

    while (!glfwWindowShouldClose(window)) {
        
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
