#ifndef _IBO_H
#define _IBO_H
#include"glad/glad.h"

class IBO{
private:
    unsigned int m_RendererID;    //ID
      // The amount of indicies inthe array

public:
    unsigned int m_IndicesCount;

    IBO(unsigned int data[], size_t size){
        glGenBuffers(1, &m_RendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        m_IndicesCount = (size/sizeof(unsigned int));

    }
    ~IBO(){
        glDeleteBuffers(1, &m_RendererID);

    }

    const void Bind(){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }
    const void UnBind(){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }


};


#endif

