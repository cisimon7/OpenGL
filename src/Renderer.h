#ifndef OPENGL_RENDERER_H
#define OPENGL_RENDERER_H

#include "glad/gl.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if (!(x)) __builtin_debugtrap()
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE_NAME__, __LINE__))

void GLClearError();

bool GLLogCall(const char *function, const char *file, int line);

class Renderer {
private:
public:
    void clear() const;
    void draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader);
};

#endif //OPENGL_RENDERER_H
