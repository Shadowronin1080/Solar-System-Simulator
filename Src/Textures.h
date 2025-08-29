#ifndef _TEXTURES_H
#define _TEXTURES_H
#include"fstream"
#include "glad/glad.h" //include glad for acces to opengl
#define STB_IMAGE_IMPLEMENTATION // implemented based on instructions given in the library
#include"stb_image.h"

#include<iostream>

class Texture {
    unsigned int m_RendererID; // unique id to use as a pointer when dealing with buffers
    const char* FilePath;  //filepath for the image
    int width, height, BPP; // local atributes for width height and bits per pixel
    unsigned char* Local_Buffer;

public:
    //Constructor
    Texture(std::string path) {
        stbi_set_flip_vertically_on_load(1);
        Local_Buffer = stbi_load(path.c_str(), &width, &height, &BPP, 4); //load image into local buffer 
        if (Local_Buffer == NULL) {
            std::cout << "Failed to load image: " << stbi_failure_reason() << std::endl;
        }
        glGenTextures(1, &m_RendererID); //generate texture
        glBindTexture(GL_TEXTURE_2D, m_RendererID);  //Bind texture
        //Set nesecary parameter
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //describes how the texture is to be scaled up
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //describes how the texture is to be scaled Down
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // ADD the data from the loaded image to the opengl texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Local_Buffer);
        UnBind();


        if (Local_Buffer) {
            stbi_image_free(Local_Buffer); // if local buffer still has data delete the data
        }



    }
    //Destructor
    ~Texture() {
        glDeleteTextures(1, &m_RendererID); //Delete the textures and free memory
    }

    void Bind(unsigned int slot = 0) {
        glActiveTexture(GL_TEXTURE0+slot);
        glBindTexture(GL_TEXTURE_2D, m_RendererID); //BInd
    }
    void UnBind() {
        glBindTexture(GL_TEXTURE_2D, 0); //UnBInd
    }
    void reconfigure(std::string path){
        glDeleteTextures(1, &m_RendererID);

         stbi_set_flip_vertically_on_load(1);
        Local_Buffer = stbi_load(path.c_str(), &width, &height, &BPP, 4); //load image into local buffer 
        if (Local_Buffer == NULL) {
            std::cout << "Failed to load image: " << stbi_failure_reason() << std::endl;
        }
        glGenTextures(1, &m_RendererID); //generate texture
        glBindTexture(GL_TEXTURE_2D, m_RendererID);  //Bind texture
        //Set nesecary parameter
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //describes how the texture is to be scaled up
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //describes how the texture is to be scaled Down
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // ADD the data from the loaded image to the opengl texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Local_Buffer);
        UnBind();


        if (Local_Buffer) {
            stbi_image_free(Local_Buffer); // if local buffer still has data delete the data
        }


    }









};



#endif