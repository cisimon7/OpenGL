#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H

#include <string>
#include "linmath.h"

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader {
private:
    std::string m_Filepath;
    unsigned int m_RendererID;

public:
    Shader(const std::string& filepath );
    ~Shader();

    void bind() const;
    void unBind() const;

    // set uniforms
    void setUniformMat4x4(int location, const mat4x4 mat);
    [[nodiscard]] int getUniformLocation(const std::string& name) const;
    [[nodiscard]] int getAttributeLocation(const std::string& name) const;

private:

    ShaderProgramSource parseShader(const std::string &filePath);

    unsigned int compileShader(unsigned int type, const char *source);

    unsigned int createShader(const std::string &vertex_text, const std::string &fragment_text);
};

#endif //OPENGL_SHADER_H
