#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) :
    m_Count(count) {
    GLCall(glGenBuffers(1, &m_RendererID)) ; /* Create for me a buffer */
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); /* Set buffer type */
    /* Creates and initializes a buffer object's data store */
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER , count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() {
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::bind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::unBind() const  {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0 ));
}
