//Vertex Shader
#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec4 color;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

out vec4 vertColor;

void main()
{
    gl_Position = (m * v * p) * vec4(pos, 1.0);
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