#include "VertexBuffer.hpp"
#include "Renderer.hpp"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    GLCall(glGenBuffers(1, &m_RendererID)); // generate buffer in gpu VRAM
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	// put data into buffer, specify the size
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    constexpr static unsigned int numOfBuffersToDelete{1u};
    GLCall(glDeleteBuffers(numOfBuffersToDelete, &m_RendererID));
}

void VertexBuffer::bind() const
{
    // array buffer, select this buffer (bind does this), size is undefined yet
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
