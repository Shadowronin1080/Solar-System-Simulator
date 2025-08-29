#ifndef _VBO_H
#define _VBO_H
#include"glad/glad.h"

class VBO {
private:
    unsigned int m_RendererID;

public:

    VBO(float data[], size_t size){
        glGenBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    }
    ~VBO(){
        glDeleteBuffers(1, &m_RendererID);

    }

    void UpdateData(float data[], size_t size){
        Bind();
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        UnBind();
    }

    void Bind(){
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }
    void UnBind(){
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }



};
#endif



