#pragma once 
#include "VertexBuffer.hpp"
#include <vector>
#include <GL/glew.h>
#include "Renderer.hpp"

struct VertexBufferElement
{
    unsigned int type; // OpenGL type 
    unsigned int count;
    unsigned char normalized;
};

class VertexBufferLayout
{
    using Elements = std::vector<VertexBufferElement>;
    using IsNormalized = unsigned char;

private:
    Elements m_Elements;
    unsigned int m_Stride;
public:
    VertexBufferLayout() : m_Stride{0} {}
    ~VertexBufferLayout(){}

    // TODO improve this 
    static constexpr unsigned int getSizeOfType(const unsigned int type)
    {
        switch (type)
        {
            case GL_FLOAT         : return sizeof(GLfloat);
            case GL_UNSIGNED_INT  : return sizeof(GLuint);
            case GL_UNSIGNED_BYTE : return sizeof(GLbyte);
        }

        ASSERT(0);
        return 0;
    }

    template<typename T>
    void push(const unsigned int count);

    // push element to layout, for example layout could look like this: int, int bool; stride -> int + int + bool
    template<>
    void push<float>(const unsigned int count)
    {
        m_Elements.push_back({GL_FLOAT, count, IsNormalized{GL_FALSE}});
        m_Stride += count * VertexBufferLayout::getSizeOfType(GL_FLOAT);     
    }

    template<>
    void push<unsigned int>(const unsigned int count)
    {
        m_Elements.push_back({GL_UNSIGNED_INT, count, IsNormalized{GL_FALSE}});
        m_Stride += count * VertexBufferLayout::getSizeOfType(GL_UNSIGNED_INT);     
    }

    template<>
    void push<unsigned char>(const unsigned int count)
    {
        m_Elements.push_back({GL_UNSIGNED_BYTE, count, IsNormalized{GL_TRUE}});
        m_Stride += count * VertexBufferLayout::getSizeOfType(GL_UNSIGNED_BYTE);
    }

    const Elements& getElements() const { return m_Elements; }
    unsigned int getStride() const { return m_Stride; }
};

class VertexArray
{
private:
    unsigned int m_RendererID;
public:
    VertexArray()
    {
        GLCall(glGenVertexArrays(1, &m_RendererID));
    }

    ~VertexArray()
    {
        GLCall(glDeleteVertexArrays(1, &m_RendererID));
    }

    void bind() const
    {
        // array buffer, select this buffer (bind does this), size is undefined yet
        GLCall(glBindVertexArray(m_RendererID));
    }

    void unbind() const
    {
        GLCall(glBindVertexArray(0));
    }

    void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
    {
        bind();
        vb.bind();
        const auto& elements = layout.getElements();
        unsigned int offset = 0;
        for(unsigned int i = 0; i < elements.size(); i++)
        {
            const auto& element = elements.at(i);
            GLCall(glEnableVertexAttribArray(i));
           /*
            * specify layout of the data in buffer; stride -> vertex to vertex
            * linking vertex buffer with vao 
            */
            GLCall(glVertexAttribPointer(i, element.count, element.type, 
                element.normalized, layout.getStride(), (const void*)offset));

            offset += element.count * VertexBufferLayout::getSizeOfType(element.type);
        }
    }
};