#ifndef OPENGL_VERTEXARRAY_H
#define OPENGL_VERTEXARRAY_H

#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray {
private:
    unsigned int m_RendererID;
public:
    VertexArray();
    ~VertexArray();

    void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

    void bind() const;

    void unBind() const;
};

#endif //OPENGL_VERTEXARRAY_H
