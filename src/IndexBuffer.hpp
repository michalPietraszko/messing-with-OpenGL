#pragma once

class IndexBuffer
{
    private:
        unsigned int m_RendererID;
        unsigned int m_Count; // indecies count
    public:
        IndexBuffer(const unsigned int* data, unsigned int count);
        ~IndexBuffer();

        void bind() const;
        void unbind() const;
        unsigned int getCount() const { return m_Count;}
};