#define GLAD_GL_IMPLEMENTATION
#define GLFW_INCLUDE_NONE

#include "GLFW/glfw3.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"


void error_callback(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

struct Vertex {
    float x, y;
    float r, g, b;
};

Vertex vertices[] = {
        {-0.5f, +0.5f, 1.f, 0.f, 0.f},
        {+0.5f, -0.5f, 0.f, 1.f, 0.f},
        {+0.5f, +0.5f, 0.f, 0.f, 1.f},
        {-0.5f, -0.5f, 0.f, 0.f, 1.f},
};

unsigned int indices[]{
        0, 1, 2,
        0, 1, 3
};

int main() {
    GLFWwindow *window;
    int mvpLocation, vPosLocation, vColLocation;

    /* Setting callback for error */
    glfwSetErrorCallback(error_callback);

    /* Initialize the library */
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "Simple Example", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    /* Creating an OpenGL context and making [window] own it
     * OpenGL context needed to be able to use OpenGL API
     * Context will remain current till another context is made or window owning the current context is destroyed */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    /* Setting what key (Esc) closes the window */
    glfwSetKeyCallback(window, key_callback);

    /* An extension library that needs access to context*/
    gladLoadGL(glfwGetProcAddress);

    /* Callback will be called immediately after the close flag has been set */
    glfwSetWindowCloseCallback(
            window,
            [](GLFWwindow *window) { fprintf(stderr, "Closing Window"); }
    );

    Shader shader("res/shaders/Basic.shader");
    shader.bind();
    mvpLocation = shader.getUniformLocation("u_MVP");
    vPosLocation = shader.getAttributeLocation("vPos");
    vColLocation = shader.getAttributeLocation("vCol");


    VertexArray vertexArray;
    VertexBuffer vertexBuffer(vertices, 4 * sizeof(Vertex));
    VertexBufferLayout layout;
    layout.Push<float>(2, vPosLocation);
    layout.Push<float>(3, vColLocation);
    vertexArray.addBuffer(vertexBuffer, layout);

    IndexBuffer indexBuffer(indices, 6);

    /* glfwGetTime() returns time since initialization */
    auto current_time = []() { return glfwGetTime(); };

    vertexArray.unBind();
    shader.unBind();
    vertexBuffer.unBind();
    indexBuffer.unBind();

    /* Checking the window close flag */
    while (!glfwWindowShouldClose(window)) {
        float ratio;
        int width, height;
        mat4x4 m, p, mvp;

        mat4x4 eye;
        mat4x4_identity(eye);

        /* Get maximum image size that can be displayed and save as width and height*/
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;

        glViewport(0, 0, width, height);  /* Create buffer of certain size */
        glClear(GL_COLOR_BUFFER_BIT); /* Clear buffer */

        mat4x4_identity(m); /* Initialization to identity matrix */
        mat4x4_rotate_Z(m, m, (float) current_time()); /* Rotating matrix by angle of time */
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f); /* Project in orthogonal view */
        mat4x4_mul(mvp, p, m); /* Final Matrix after rotation and projection */

        shader.bind();

        /* Uniforms are used to send data to shaders from cpu
         * Uniforms are set per draw where attributes are set per vertex  */
        shader.setUniformMat4x4(mvpLocation, mvp);

        vertexArray.bind();
        indexBuffer.bind();

        // glDrawArrays(GL_TRIANGLES, 0, 6);
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        /* Swapping of buffers after each frame has been rendered */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /* When a window is no longer needed, destroy it */
    GLCall(glfwDestroyWindow(window));

    /* Terminate the Library */
    glfwTerminate();
    exit(EXIT_SUCCESS);
}