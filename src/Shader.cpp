#include <fstream>
#include <sstream>
#include <iostream>
#include "Shader.h"
#include "glad/gl.h"
#include "Renderer.h"

Shader::Shader(const std::string &filepath) : m_Filepath(filepath), m_RendererID(0) {
    const auto[vertex_shader_text, fragment_shader_text] =
    parseShader(filepath);
    m_RendererID = createShader(vertex_shader_text, fragment_shader_text);
}

Shader::~Shader() {
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::bind() const {
    GLCall(glUseProgram(m_RendererID));
}

void Shader::unBind() const {
    GLCall(glUseProgram(0));
}

void Shader::setUniformMat4x4(int location, const mat4x4 mat) {
    GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat *) mat));
}

int Shader::getUniformLocation(const std::string &name) const {
    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1) {
        std::cout << "Warning: uniform " << name << " doesn't exist!" << std::endl;
    }
    return location;
}

ShaderProgramSource Shader::parseShader(const std::string &filePath) {
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


unsigned int Shader::compileShader(unsigned int type, const char *source) {

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


unsigned int Shader::createShader(const std::string &vertex_text, const std::string &fragment_text) {

    unsigned int vertex_shader, fragment_shader, program;

    const char *vText = vertex_text.c_str();
    const char *fText = fragment_text.c_str();
    vertex_shader = compileShader(GL_VERTEX_SHADER, vText);
    fragment_shader = compileShader(GL_FRAGMENT_SHADER, fText);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}

int Shader::getAttributeLocation(const std::string &name) const {
    GLCall(int location = glGetAttribLocation(m_RendererID, name.c_str()));
    if (location == -1) {
        std::cout << "Warning: attribute " << name << " doesn't exist!" << std::endl;
    }
    return location;

}
