//Vertex Shader
//==================================================================================
#version 120

//Vertex Attributes
attribute vec3 pos;
attribute vec3 norm;
attribute vec3 tangent;
attribute vec2 uv;
attribute vec4 group;
attribute vec4 weight;

//Matrices
uniform mat4 m;
uniform mat4 v;
uniform mat4 p;
uniform mat4 n;
uniform mat4 bones[20];

//Shader Outputs
varying vec2 texCoord0;
varying vec3 fragPos;
varying vec3 normal;
varying mat3 TBN;


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
    
    //Calculate TBN matrix
    vec3 T = normalize(vec3(m * vec4(tangent, 0.0)));
    vec3 N = normalize(vec3(m * vec4(norm, 0.0)));
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    TBN = mat3(T, B, N);
    
    //Calculate frag pos and normal
    fragPos = vec3(m * tmp);
    normal = mat3(n) * norm;
}
//==================================================================================
//End Vertex Shader


//Fragment Shader
//==================================================================================
#version 120

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
varying vec2 texCoord0;
varying vec3 fragPos;
varying vec3 normal;
varying mat3 TBN;

//Shader Uniforms
uniform vec3 camPos;
uniform Light light;
uniform Material mat;
uniform sampler2D tex0;
uniform sampler2D tex1;


//Entry Point
void main()
{
    //Fetch texture color
    vec4 color0 = texture2D(tex0, texCoord0);
    
    //Fetch per fragment normal
    vec3 normal = texture2D(tex1, texCoord0).rgb;
    normal = normal * 2.0 - 1.0;
    
    //Calculate ambient color
    vec3 ambient = light.ambient * mat.ambient;
    
    //Calculate diffuse color
    vec3 norm = normalize(TBN * normal);
    vec3 lightDir = normalize(light.pos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * mat.diffuse);
    
    //Calculate specular color
    vec3 viewDir = normalize(camPos - fragPos);
    vec3 reflectDir = reflect(-viewDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
    vec3 specular = light.specular * (spec * mat.specular);
    
    //Calculate fragment color
    gl_FragColor = color0 * vec4(ambient + diffuse + specular, 1.0);
}
//==================================================================================
//End Fragment Shader


//ES Vertex Shader
//==================================================================================
#version 100

precision mediump float;

//Vertex Attributes
attribute vec3 pos;
attribute vec3 norm;
attribute vec3 tangent;
attribute vec2 uv;
attribute vec4 group;
attribute vec4 weight;

//Matrices
uniform mat4 m;
uniform mat4 v;
uniform mat4 p;
uniform mat4 n;
uniform mat4 bones[20];

//Shader Outputs
varying vec2 texCoord0;
varying vec3 fragPos;
varying vec3 normal;
varying mat3 TBN;


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
    
    //Calculate TBN matrix
    vec3 T = normalize(vec3(m * vec4(tangent, 0.0)));
    vec3 N = normalize(vec3(m * vec4(norm, 0.0)));
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    TBN = mat3(T, B, N);
    
    //Calculate frag pos and normal
    fragPos = vec3(m * tmp);
    normal = mat3(n) * norm;
}
//==================================================================================
//End ES Vertex Shader


//ES Fragment Shader
//==================================================================================
#version 100

precision mediump float;

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
varying vec2 texCoord0;
varying vec3 fragPos;
varying vec3 normal;
varying mat3 TBN;

//Shader Uniforms
uniform vec3 camPos;
uniform Light light;
uniform Material mat;
uniform sampler2D tex0;
uniform sampler2D tex1;


//Entry Point
void main()
{
    //Fetch texture color
    vec4 color0 = texture2D(tex0, texCoord0);
    
    //Fetch per fragment normal
    /* vec3 normal = texture2D(tex1, texCoord0).rgb;
    normal = normal * 2.0 - 1.0; */
    
    //Calculate ambient color
    vec3 ambient = light.ambient * mat.ambient;
    
    //Calculate diffuse color
    vec3 norm = normalize(normal); //normalize(TBN * normal);
    vec3 lightDir = normalize(light.pos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * mat.diffuse);
    
    //Calculate specular color
    vec3 viewDir = normalize(camPos - fragPos);
    vec3 reflectDir = reflect(-viewDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
    vec3 specular = light.specular * (spec * mat.specular);
    
    //Calculate fragment color
    gl_FragColor = color0 * vec4(ambient + diffuse + specular, 1.0);
}
//==================================================================================
//End ES Fragment Shader