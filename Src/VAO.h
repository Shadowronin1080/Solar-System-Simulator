#ifndef _VAO_H
#define _VAO_H
#include"glad/glad.h"
#include"VBO.h"

struct BufferElementLayout
{
    unsigned int Index;
    int Attrib_length;
    GLenum type;
    bool need_normalisation;
    GLsizei stride;
    const void* offset;
};

class VAO
{
private:
    unsigned int m_RendererID;
public:
    VAO(){
        glGenVertexArrays(1, &m_RendererID);
    };
    ~VAO(){
        glDeleteVertexArrays(1, &m_RendererID);
    };

    void Bind(){
        glBindVertexArray(m_RendererID);
    }
    void UnBind(){
        glBindVertexArray(0);
    }

    void AddAtribute(VBO& VertexBuffer, BufferElementLayout layout){
        Bind();
        VertexBuffer.Bind();
        glVertexAttribPointer(layout.Index, layout.Attrib_length, layout.type, layout.need_normalisation, layout.stride, layout.offset);
        glEnableVertexAttribArray(layout.Index);
        VertexBuffer.UnBind();
        UnBind();
    }

};



#endif

