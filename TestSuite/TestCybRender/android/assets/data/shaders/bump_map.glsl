//Vertex Shader
//==================================================================================
#version 330 core

//Vertex Attributes
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 uv;
layout (location = 4) in vec4 group;
layout (location = 5) in vec4 weight;

//Matrices
uniform mat4 m;
uniform mat4 v;
uniform mat4 p;
uniform mat4 n;
uniform mat4 bones[20];

//Shader Outputs
out vec2 texCoord0;
out vec3 fragPos;
out vec3 normal;


//Entry Point
void main()
{
    //Apply bone transform
    vec4 tmp = vec4(pos, 1.0);
    
    if(group.x > -1.0)
    {
        tmp = bones[int(group.x)] * tmp;
    }
    
    //Calculate final vertex position and texture coords
    gl_Position = (p * v * m) * tmp;
    texCoord0 = uv;
    
    //Calculate frag pos and normal
    fragPos = vec3(m * tmp);
    normal = mat3(n) * norm;
}
//==================================================================================
//End Vertex Shader


//Fragment Shader
//==================================================================================
#version 330 core

//Structures
struct Light
{
    vec3 pos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};


//Shader Inputs
in vec2 texCoord0;
in vec3 fragPos;
in vec3 normal;

//Shader Uniforms
uniform vec3 camPos;
uniform Light light;
uniform Material mat;
uniform sampler2D tex0;

//Shader Outputs
out vec4 fragColor;


//Entry Point
void main()
{
    //Fetch texture color
    vec4 color0 = texture(tex0, texCoord0);
    
    //Calculate ambient color
    vec3 ambient = light.ambient * mat.ambient;
    
    //Calculate diffuse color
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.pos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * mat.diffuse);
    
    //Calculate specular color
    vec3 viewDir = normalize(camPos - fragPos);
    vec3 reflectDir = reflect(-viewDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
    vec3 specular = light.specular * (spec * mat.specular);
    
    //Calculate fragment color
    fragColor = color0 * vec4(ambient + diffuse + specular, 1.0);
}
//==================================================================================
//End Fragment Shader


//ES Vertex Shader
//==================================================================================
#version 300 es

//Vertex Attributes
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 uv;
layout (location = 4) in vec4 group;
layout (location = 5) in vec4 weight;

//Matrices
uniform mat4 m;
uniform mat4 v;
uniform mat4 p;
uniform mat4 n;
uniform mat4 bones[20];

//Shader Outputs
out vec2 texCoord0;
out vec2 texCoord1;
out vec3 fragPos;
out vec3 normal;


//Entry Point
void main()
{
    //Apply bone transform
    vec4 tmp = vec4(pos, 1.0);
    
    if(group.x > -1.0)
    {
        tmp = bones[int(group.x)] * tmp;
    }
    
    //Calculate final vertex position and texture coords
    gl_Position = (p * v * m) * tmp;
    texCoord0 = uv;
    
    //Calculate frag pos and normal
    fragPos = vec3(m * tmp);
    normal = mat3(n) * norm;
}
//==================================================================================
//End ES Vertex Shader


//ES Fragment Shader
//==================================================================================
#version 300 es

//Structures
struct Light
{
    vec3 pos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};


//Shader Inputs
in vec2 texCoord0;
in vec2 texCoord1;
in vec3 fragPos;
in vec3 normal;

//Shader Uniforms
uniform vec3 camPos;
uniform Light light;
uniform Material mat;
uniform sampler2D tex0;
uniform sampler2D tex1;

//Shader Outputs
out vec4 fragColor;


//Entry Point
void main()
{
    //Fetch texture color
    vec4 color0 = texture(tex0, texCoord0);
    
    //Calculate ambient color
    vec3 ambient = light.ambient * mat.ambient;
    
    //Calculate diffuse color
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.pos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * mat.diffuse);
    
    //Calculate specular color
    vec3 viewDir = normalize(camPos - fragPos);
    vec3 reflectDir = reflect(-viewDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
    vec3 specular = light.specular * (spec * mat.specular);
    
    //Calculate fragment color
    fragColor = color0 * vec4(ambient + diffuse + specular, 1.0);
}
//==================================================================================
//End ES Fragment Shader