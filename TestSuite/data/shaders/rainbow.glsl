//Vertex Shader
#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 color;

out vec4 vertColor;

void main()
{
    gl_Position = vec4(pos, 1.0);
    vertColor = color;
}
//End Vertex Shader

//Fragment Shader
#version 330 core

in vec4 vertColor;

out vec4 fragColor;

void main()
{
    fragColor = vertColor;
}
//End Fragment Shader