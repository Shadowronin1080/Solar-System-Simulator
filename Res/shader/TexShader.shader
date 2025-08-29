#Shader Vertex

#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 Texcoords;

out vec2 v_Texcoords;

uniform mat4 u_matrix;
        
void main()
{
    gl_Position = u_matrix*position;
    v_Texcoords = Texcoords;

};

#Shader fragment


#version 460 core
        
layout(location = 0) out vec4 colour;

in vec2 v_Texcoords;

uniform vec4 u_colour;
uniform sampler2D u_Texture;

void main()
{
    vec4 Texcolour = texture(u_Texture, v_Texcoords);
    colour = Texcolour;
};

