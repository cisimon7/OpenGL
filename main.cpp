#define GLAD_GL_IMPLEMENTATION
#include "glad/gl.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "linmath.h"

#include <cstdlib>
#include <cstdio>


void error_callback(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

struct Pixel {
    float x, y;
    float r, g, b;
};

Pixel vertices[3] = {
        {-0.6f, -0.4f, 1.f, 0.f, 0.f},
        {+0.6f, -0.4f, 0.f, 1.f, 0.f},
        {+0.0f, +0.6f, 0.f, 0.f, 1.f}
};

static const char* vertex_shader_text =
        "#version 110\n"
        "uniform mat4 MVP;\n"
        "attribute vec3 vCol;\n"
        "attribute vec2 vPos;\n"
        "varying vec3 color;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
        "    color = vCol;\n"
        "}\n";

static const char* fragment_shader_text =
        "#version 110\n"
        "varying vec3 color;\n"
        "void main()\n"
        "{\n"
        "    gl_FragColor = vec4(color, 1.0);\n"
        "}\n";

int main() {
    GLFWwindow *window;
    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    GLuint mvp_location, vpos_location, vcol_location;

    /* Setting callback for error */
    glfwSetErrorCallback(error_callback);

    /* Initialize the library */
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(640, 480, "Simple Example", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    /* Creating an OpenGL context and making [window] own it
     * OpenGL context needed to be able to use OpenGL API
     * Context will remain current till another context is made or window owning the current context is destroyed */
    glfwMakeContextCurrent(window);

    /* Setting what key (Esc) closes the window */
    glfwSetKeyCallback(window, key_callback);

    /* An extension library that needs access to context*/
    gladLoadGL(glfwGetProcAddress);

    /* Callback will be called immediately after the close flag has been set */
    glfwSetWindowCloseCallback(
            window,
            [](GLFWwindow *window) { fprintf(stderr, "Closing Window"); }
    );

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, nullptr);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, nullptr);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    mvp_location = glGetUniformLocation(program, "MVP");
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void*) 0);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void*) (sizeof(float) * 2));

    /* glfwGetTime() returns time since initialization */
    auto current_time = []() { return glfwGetTime(); };

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

        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat *) mvp);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swapping of buffers after each frame has been rendered */
        glfwSwapBuffers(window);
        // glfwSwapInterval(1);
        glfwPollEvents();
    }

    /* When a window is no longer needed, destroy it */
    glfwDestroyWindow(window);

    /* Terminate the Library */
    glfwTerminate();
    exit(EXIT_SUCCESS);
}