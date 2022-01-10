#ifndef OPENGL_VERTEXBUFFERLAYOUT_H
#define OPENGL_VERTEXBUFFERLAYOUT_H

#include <vector>
#include "Renderer.h"

struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    unsigned char normalised;
    unsigned int location;

    static unsigned int getSizeOfType(unsigned int type) {
        switch (type) {
            case GL_FLOAT:
                return sizeof(float);
            case GL_UNSIGNED_INT:
                return sizeof(float);
            case GL_UNSIGNED_BYTE:
                return sizeof(char);
        }
        ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout {
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;
public:
    VertexBufferLayout() : m_Stride(0) {};

    template<typename T>
    void Push(unsigned int count, unsigned int location);

    template<>
    void Push<float>(unsigned int count, unsigned int location) {
        m_Elements.push_back({GL_FLOAT, count, GL_FALSE, location});
        m_Stride += VertexBufferElement::getSizeOfType(GL_FLOAT) * count;
    }

    template<>
    void Push<unsigned int>(unsigned int count, unsigned int location) {
        m_Elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE, location});
        m_Stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT) * count;
    }

    template<>
    void Push<unsigned char>(unsigned int count, unsigned int location) {
        m_Elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE, location});
        m_Stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE) * count;
    }

    inline const std::vector<VertexBufferElement> GetElement() const { return m_Elements; }

    inline unsigned int getStride() const { return m_Stride; }

    ~VertexBufferLayout() = default;;
};

#endif //OPENGL_VERTEXBUFFERLAYOUT_H
