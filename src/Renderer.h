#ifndef OPENGL_RENDERER_H
#define OPENGL_RENDERER_H

#include "glad/gl.h"

#define ASSERT(x) if (!(x)) __builtin_debugtrap()
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE_NAME__, __LINE__))

void GLClearError();

bool GLLogCall(const char *function, const char *file, int line);

#endif //OPENGL_RENDERER_H
