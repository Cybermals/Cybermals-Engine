//Vertex Shader
#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 uv;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

out vec2 texCoord0;
out vec2 texCoord1;

void main()
{
    gl_Position = (p * v * m) * vec4(pos, 1.0);
    texCoord0 = vec2(uv.x * 4.0, uv.y * 4.0);
    texCoord1 = uv;
}
//End Vertex Shader

//Fragment Shader
#version 330 core

in vec2 texCoord0;
in vec2 texCoord1;

uniform sampler2D tex0;
uniform sampler2D tex1;

out vec4 fragColor;

void main()
{
    vec4 color0 = texture(tex0, texCoord0);
    vec4 color1 = texture(tex1, texCoord1);
    
    if(color1.w < .1)
    {
        discard;
    }
    
    fragColor = color0 * color1;
}
//End Fragment Shader