#ifndef _SHADERS_H
#define _SHADERS_H
#include<iostream>
#include<fstream>
#include<sstream>
#include"glad/glad.h"
#include"glm/glm.hpp"


struct ShaderSource {
    std::string VertexShader;
    std::string FragmentShader;
};

class Shaders
{
private:
    unsigned int m_RendererID;
    ShaderSource source;

public:
    Shaders(const char* path) {
        source = ParseShader(path);
        m_RendererID = CreateShader((source.VertexShader).c_str(), (source.FragmentShader).c_str());
    };
    ~Shaders() {
        glDeleteProgram(m_RendererID);

    };

    unsigned int CompileShader(unsigned int type, const char* source);
    ShaderSource ParseShader(const std::string& filepath);
    unsigned int CreateShader(const char* vertexshader, const char* fragmentshader);

    void Bind() {
        glUseProgram(m_RendererID);
    }
    void UnBind() {
        glUseProgram(0);
    }

    int GetUniformLocation(const char* uniform_name) {
        int location = glGetUniformLocation(m_RendererID, uniform_name);
        return location;
    }
    void SetUniform1i(const char* unifom_name, int i1) {
        Bind();
        glUniform1i(GetUniformLocation(unifom_name), i1);
        UnBind();
    }
    void SetUniform4f(const char* unifom_name, float f1, float f2, float f3, float f4) {
        Bind();
        glUniform4f(GetUniformLocation(unifom_name), f1, f2, f3, f4);
        UnBind();
    }
    void SetUniformMat4f(const char* unifom_name, glm::mat4& matrix) {
        Bind();
        glUniformMatrix4fv(GetUniformLocation(unifom_name), 1, GL_FALSE, &matrix[0][0]);
        UnBind();
    }
};



ShaderSource Shaders::ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);
    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {

        if (line.find("#Shader") != std::string::npos) {
            if (line.find("Vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else { type = ShaderType::FRAGMENT; }
        }
        else
        {
            ss[(int)type] << line << std::endl;

        }
    }
    return { ss[0].str(),ss[1].str() };
}
unsigned int Shaders::CompileShader(unsigned int type, const char* source) {
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &source, nullptr);
    glCompileShader(id);

    //ERROR HANDELING
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " Shaders " << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shaders::CreateShader(const char* vertexshader, const char* fragmentshader) {
    unsigned int program = glCreateProgram();

    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexshader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentshader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;

}


#endif