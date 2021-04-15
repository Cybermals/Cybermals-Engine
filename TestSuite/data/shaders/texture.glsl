//Vertex Shader
#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 uv;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

out vec2 texCoord;

void main()
{
    gl_Position = (p * v * m) * vec4(pos, 1.0);
    texCoord = uv;
}
//End Vertex Shader

//Fragment Shader
#version 330 core

in vec2 texCoord;

uniform sampler2D tex0;

out vec4 fragColor;

void main()
{
    fragColor = texture(tex0, texCoord);
}
//End Fragment Shader