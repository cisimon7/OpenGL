#ifndef OPENGL_INDEXBUFFER_H
#define OPENGL_INDEXBUFFER_H

class IndexBuffer {
private:
    unsigned int m_RendererID;
    unsigned int m_Count;
public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();

    void bind() const;
    void unBind() const;

    inline unsigned int getCount() const { return m_Count;  }
};

#endif //OPENGL_INDEXBUFFER_H
