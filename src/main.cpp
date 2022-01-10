#define GLAD_GL_IMPLEMENTATION
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "linmath.h"

#include <sstream>
#include <iostream>
#include <fstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"


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

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource parseShader(const std::string &filePath) {
    std::ifstream stream(filePath);
    std::stringstream shaders[2];

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {

        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        } else {
            shaders[(int) type] << line << "\n";
        }
    }

    return {shaders[(int) ShaderType::VERTEX].str(), shaders[(int) ShaderType::FRAGMENT].str()};
}


static unsigned int compile_shader(unsigned int type, const char *source) {

    unsigned int shader;

    shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *) alloca(length * sizeof(char));
        glGetShaderInfoLog(shader, length, &length, message);

        std::cout << "Failed to compile: "
                  << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " Shader"
                  << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

static unsigned int create_shader(const std::string &vertex_text, const std::string &fragment_text) {

    unsigned int vertex_shader, fragment_shader, program;

    const char *vText = vertex_text.c_str();
    const char *fText = fragment_text.c_str();
    vertex_shader = compile_shader(GL_VERTEX_SHADER, vText);
    fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fText);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}

int main() {
    GLFWwindow *window;
    unsigned int program, vertex_array;
    GLuint mvp_location, vpos_location, vcol_location;

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

    GLCall(glGenVertexArrays(1, &vertex_array));
    GLCall(glBindVertexArray(vertex_array));

    VertexBuffer vertexBuffer(vertices, 4 * sizeof(Vertex));

    const auto[vertex_shader_text, fragment_shader_text] =
    parseShader("res/shaders/Basic.shader");
    program = create_shader(vertex_shader_text, fragment_shader_text);

    mvp_location = glGetUniformLocation(program, "u_MVP");
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");

    GLCall(glEnableVertexAttribArray(vpos_location));
    GLCall(glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), nullptr));
    GLCall(glEnableVertexAttribArray(vcol_location));
    GLCall(glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void *) (sizeof(float) * 2)));

    IndexBuffer indexBuffer(indices, 6);

    GLCall(glUseProgram(program));

    /* glfwGetTime() returns time since initialization */
    auto current_time = []() { return glfwGetTime(); };


    GLCall(glBindVertexArray(0));
    GLCall(glUseProgram(0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

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

        GLCall(glUseProgram(program));

        /* Uniforms are used to send data to shaders from cpu
         * Uniforms are set per draw where attributes are set per vertex  */
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat *) mvp);

        // glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_size);

        GLCall(glBindVertexArray(vertex_array));
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_count);
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