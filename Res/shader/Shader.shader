#Shader Vertex

#version 460 core

layout(location = 0) in vec4 position;

uniform mat4 u_matrix;
        
void main()
{
    gl_Position = u_matrix*position;
};

#Shader fragment


#version 460 core
        
layout(location = 0) out vec4 colour;

uniform vec4 u_colour;

void main()
{
    colour = u_colour;
};

