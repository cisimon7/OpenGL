#include "VertexArray.h"
#include "Renderer.h"

VertexArray::VertexArray() {
    GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray() {
    GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout) {
    bind();
    vb.bind();
    const auto &elements = layout.GetElement();

    unsigned int offset = 0;

    for (auto element: elements) {
        GLCall(glEnableVertexAttribArray(element.location));
        GLCall(glVertexAttribPointer(
                element.location,
                element.count,
                element.type,
                element.normalised,
                layout.getStride(),
                (const void *) offset
        ));
        offset += element.count * VertexBufferElement::getSizeOfType(element.type);
    }
}

void VertexArray::bind() const {
    GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::unBind() const {
    GLCall(glBindVertexArray(0));
}
