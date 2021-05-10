//Vertex Shader
//=================================================================================
#version 120

//Vertex Attributes
attribute vec3 pos;
attribute vec3 norm;
attribute vec4 color;

//Matrices
uniform mat4 m;
uniform mat4 v;
uniform mat4 p;
uniform mat4 n;

//Shader Outputs
varying vec4 vertColor;
varying vec3 fragPos;
varying vec3 normal;


//Entry Point
void main()
{
    gl_Position = (p * v * m) * vec4(pos, 1.0);
    vertColor = color;
    fragPos = vec3(m * vec4(pos, 1.0));
    normal = mat3(n) * norm;
}
//=================================================================================
//End Vertex Shader


//Fragment Shader
//=================================================================================
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
varying vec4 vertColor;
varying vec3 fragPos;
varying vec3 normal;

//Shader Uniforms
uniform vec3 camPos;
uniform Light light;
uniform Material mat;


//Entry Point
void main()
{
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
    gl_FragColor = vertColor * vec4(ambient + diffuse + specular, 1.0);
}
//=================================================================================
//End Fragment Shader


//ES Vertex Shader
//=================================================================================
#version 100

precision mediump float;

//Vertex Attributes
attribute vec3 pos;
attribute vec3 norm;
attribute vec4 color;

//Matrices
uniform mat4 m;
uniform mat4 v;
uniform mat4 p;
uniform mat4 n;

//Shader Outputs
varying vec4 vertColor;
varying vec3 fragPos;
varying vec3 normal;


//Entry Point
void main()
{
    gl_Position = (p * v * m) * vec4(pos, 1.0);
    vertColor = color;
    fragPos = vec3(m * vec4(pos, 1.0));
    normal = mat3(n) * norm;
}
//=================================================================================
//End ES Vertex Shader


//ES Fragment Shader
//=================================================================================
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
varying vec4 vertColor;
varying vec3 fragPos;
varying vec3 normal;

//Shader Uniforms
uniform vec3 camPos;
uniform Light light;
uniform Material mat;


//Entry Point
void main()
{
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
    gl_FragColor = vertColor * vec4(ambient + diffuse + specular, 1.0);
}
//=================================================================================
//End ES Fragment Shader