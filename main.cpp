#include <GLFW/glfw3.h>
#include <cstdio>


void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

int main() {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(640, 480, "Window Title", nullptr, nullptr);
    if (!window) {

    }

    /* When a window is no longer needed, destroy it */
    glfwDestroyWindow(window);

    /* Terminate the Library */
    glfwTerminate();
    return 0;
}