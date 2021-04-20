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
out vec3 fragPos;
out vec3 normal;

void main()
{
    gl_Position = (p * v * m) * vec4(pos, 1.0);
    texCoord0 = vec2(uv.x * 4.0, uv.y * 4.0);
    texCoord1 = uv;
    fragPos = vec3(m * vec4(pos, 1.0));
    normal = mat3(transpose(inverse(m))) * norm;
}
//End Vertex Shader

//Fragment Shader
#version 330 core

in vec2 texCoord0;
in vec2 texCoord1;
in vec3 fragPos;
in vec3 normal;

uniform vec3 camPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float ambientStrength;
uniform float specularStrength;
uniform sampler2D tex0;
uniform sampler2D tex1;

out vec4 fragColor;

void main()
{
    //Fetch texture colors
    vec4 color0 = texture(tex0, texCoord0);
    vec4 color1 = texture(tex1, texCoord1);
    
    //Discard transparent fragments
    if(color0.w < .1 || color1.w < .1)
    {
        discard;
    }
    
    //Calculate ambient color
    vec3 ambient = ambientStrength * lightColor;
    
    //Calculate diffuse color
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    //Calculate specular color
    vec3 viewDir = normalize(camPos - fragPos);
    vec3 reflectDir = reflect(-viewDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
    
    //Calculate fragment color
    fragColor = (color0 * color1) * vec4(ambient + diffuse + specular, 1.0);
}
//End Fragment Shader