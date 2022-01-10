#ifndef OPENGL_VERTEXBUFFER_H
#define OPENGL_VERTEXBUFFER_H

class VertexBuffer {
private:
    unsigned int m_RendererID;
public:
     VertexBuffer(const void* data, unsigned int size);
     ~VertexBuffer();

     void bind() const;
     void unBind() const;
};

#endif //OPENGL_VERTEXBUFFER_H
