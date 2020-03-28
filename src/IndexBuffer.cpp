#include "IndexBuffer.hpp"
#include "Renderer.hpp"

// data?
IndexBuffer::IndexBuffer(const unsigned int* data, const unsigned int count) : m_Count{count}
{
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    constexpr static unsigned int numOfBuffersToDelete{1u};
    GLCall(glDeleteBuffers(numOfBuffersToDelete, &m_RendererID));
}

void IndexBuffer::bind() const
{
    // array buffer, select this buffer (bind does this), size is undefined yet
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
