#ifndef _RENDERER_H
#define _RENDERER_H
#include<iostream>
#include <vector>
#include"glad/glad.h"
#include"VAO.h"
#include"IBO.h"
#include"Shaders.h"
#include"Textures.h"

#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"

const int Elipse_verticies_count = 100;
const int Point_verticies_count = 100;

void GLAPIENTRY MessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParm) {

    printf("OpenGL Error (%d): %s\n", id, message);
    printf("    Source: %s\n", source == GL_DEBUG_SOURCE_API ? "API" :
        source == GL_DEBUG_SOURCE_WINDOW_SYSTEM ? "Window System" :
        source == GL_DEBUG_SOURCE_SHADER_COMPILER ? "Shader Compiler" :
        source == GL_DEBUG_SOURCE_THIRD_PARTY ? "Third Party" :
        source == GL_DEBUG_SOURCE_APPLICATION ? "Application" :
        source == GL_DEBUG_SOURCE_OTHER ? "Other" : "Unknown");
    printf("    Type: %s\n", type == GL_DEBUG_TYPE_ERROR ? "Error" :
        type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR ? "Deprecated Behavior" :
        type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR ? "Undefined Behavior" :
        type == GL_DEBUG_TYPE_PORTABILITY ? "Portability" :
        type == GL_DEBUG_TYPE_PERFORMANCE ? "Performance" :
        type == GL_DEBUG_TYPE_OTHER ? "Other" : "Unknown");
    printf("    Severity: %s\n", severity == GL_DEBUG_SEVERITY_HIGH ? "High" :
        severity == GL_DEBUG_SEVERITY_MEDIUM ? "Medium" :
        severity == GL_DEBUG_SEVERITY_LOW ? "Low" : "Unknown");

}

class RendererObject
{
public:
    VAO va;
    IBO ib;
    VBO vb_pos, vb_tex;
    Shaders shader_program;
    unsigned int RendererIndex;

    RendererObject(unsigned int index, unsigned int indecies[] = NULL, size_t indecies_size = NULL, float pos_data[] = NULL, float tex_data[] = NULL, size_t size = NULL, const char* shader_path = "C:/dev/Solar System Simulator/Res/shader/Shader.shader")
        : ib(indecies, indecies_size), vb_pos(pos_data, size), vb_tex(tex_data, size), shader_program(shader_path), va()
    {

        RendererIndex = index;

    };
    ~RendererObject() {

    }
    void AddPosAtribute(BufferElementLayout layout) {
        va.AddAtribute(vb_pos, layout);
    }
    void AddTexAtribute(BufferElementLayout layout) {
        va.AddAtribute(vb_tex, layout);
    }
};


class Renderer
{
private:

public:
    Renderer() {};
    ~Renderer() {
    };

    void CustomElementsDraw(RendererObject& obj, GLenum mode) {
        obj.shader_program.Bind();
        obj.va.Bind();
        obj.ib.Bind();
        glDrawElements(mode, obj.ib.m_IndicesCount, GL_UNSIGNED_INT, 0);
        obj.shader_program.UnBind();
        obj.va.UnBind();
        obj.ib.UnBind();
    }
    void DrawLoop(RendererObject& obj) {
        obj.shader_program.Bind();
        obj.va.Bind();
        glDrawArrays(GL_LINE_LOOP, 0, Elipse_verticies_count);
        obj.shader_program.UnBind();
        obj.va.UnBind();
    }
    void DrawRectangle(RendererObject& rectangle, Texture* tex = nullptr) {
        if (!(tex == nullptr)) { (*tex).Bind(); }
        rectangle.shader_program.Bind();
        rectangle.va.Bind();
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        rectangle.shader_program.UnBind();
        if (!(tex == nullptr)) { (*tex).UnBind(); }
    }
    std::vector<float> CircleCalc(float x, float y, float radius, int count = Point_verticies_count) {

        const long double pi = 3.14159265359;
        //the difference by witch theta shall increment
        long double dtheta = (2 * pi) / count;
        //create a vector (variable array) and pushback the coordinates of the centere to it
        std::vector<float> vec;
        vec.push_back(x);
        vec.push_back(y);


        // use loop to calculate the position coordinates of the points on circumfrence
        for (double theta = 0; theta <= (2 * pi); theta += dtheta)
        {
            vec.push_back((x + (radius * cos(theta))));
            vec.push_back((y + (radius * sin(theta))));

        }
        vec.push_back((x + (radius * cos(0))));
        vec.push_back((y + (radius * sin(0))));


        return vec;

    }

    std::vector<float> CircleTex(const std::vector<float>& positions) {
        std::vector<float> texCoords;

        // Find min and max values for x and y
        float minX = positions[0], minY = positions[1], maxX = positions[0], maxY = positions[1];
        for (size_t i = 2; i < positions.size(); i += 2) {
            minX = std::min(minX, positions[i]);
            minY = std::min(minY, positions[i + 1]);
            maxX = std::max(maxX, positions[i]);
            maxY = std::max(maxY, positions[i + 1]);
        }

        // Calculate range for normalization
        float rangeX = maxX - minX;
        float rangeY = maxY - minY;

        // Normalize coordinates
        for (size_t i = 0; i < positions.size(); i += 2) {
            float x = (positions[i] - minX) / rangeX;
            float y = (positions[i + 1] - minY) / rangeY;
            texCoords.push_back(x);
            texCoords.push_back(y);
        }

        return texCoords;
    }


    void DrawPoint(RendererObject& circle, int count = Point_verticies_count, Texture* tex = nullptr) {
        if (!(tex == nullptr)) { (*tex).Bind(); }
        circle.shader_program.Bind();
        circle.va.Bind();

        glDrawArrays(GL_TRIANGLE_FAN, 0, (2 + count));
        circle.shader_program.UnBind();
        circle.va.UnBind();
        if (!(tex == nullptr)) { (*tex).UnBind(); }

    }
    void DrawLine(RendererObject& line, int count = 2) {
        line.shader_program.Bind();
        line.va.Bind();

        glDrawArrays(GL_LINES, 0, count);

        line.shader_program.UnBind();
        line.va.UnBind();
    }
    void DrawLineStrip(RendererObject& line, int count = 2) {
        line.shader_program.Bind();
        line.va.Bind();

        glDrawArrays(GL_LINE_STRIP, 0, count);

        line.shader_program.UnBind();
        line.va.UnBind();
    }
    void DrawLineLoop(RendererObject& line, int count = 2) {
        line.shader_program.Bind();
        line.va.Bind();

        glDrawArrays(GL_LINE_LOOP, 0, count);

        line.shader_program.UnBind();
        line.va.UnBind();
    }

    template<typename T> // code to apply the progection matric
    void ApplyMatrix(T& obj, glm::mat4 matrix) {
        obj.shader_program.SetUniformMat4f("u_matrix", matrix);
    }

    template<typename T> // base case to terminate recursion
    void SetProjectionMatrix(glm::mat4& matrix, T& first) {
        ApplyMatrix(first, matrix);
    }

    template<typename T, typename... Rest>
    void SetProjectionMatrix(glm::mat4& matrix, T& first, Rest&... rest) {

        ApplyMatrix(first, matrix); // apply matrix to first arg 
        SetProjectionMatrix(matrix, rest...); // recursively call function for all arguments given
    }


};






#endif
