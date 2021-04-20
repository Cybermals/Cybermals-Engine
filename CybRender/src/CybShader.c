/*
CybRender - Shader API
*/

#include <stdlib.h>

#include "CybObjects.h"
#include "CybShader.h"


//Structures
//=================================================================================
struct Cyb_Shader
{
    Cyb_Object base;
    Cyb_Renderer *renderer;
    GLuint program;
};


typedef struct
{
    Cyb_ListNode base;
    char *id;
    Cyb_Shader *shader;
} Cyb_ShaderCacheNode;


//Globals
//=================================================================================
static Cyb_List *shaderCache = NULL;
char infoLog[512];


//Functions
//=================================================================================
static void Cyb_FreeShaderCache(void)
{
    //This currently causes a crash if it gets called after the OpenGL context
    //was already destroyed.
    if(shaderCache)
    {
        Cyb_FreeObject((Cyb_Object**)shaderCache);
    }
}


static void Cyb_FreeShader(Cyb_Shader *shader)
{
    //Deselect the shader program
    Cyb_GLExtAPI *glExtAPI = Cyb_GetGLExtAPI(shader->renderer);
    Cyb_SelectRenderer(shader->renderer);
    glExtAPI->UseProgram(0);
    
    //Delete the shader program
    if(shader->program)
    {
        glExtAPI->DeleteProgram(shader->program);
    }
}


static void Cyb_FreeShaderCacheNode(Cyb_ShaderCacheNode *node)
{
    //Free the ID string
    if(node->id)
    {
        SDL_free(node->id);
    }
    
    //Free the shader
    if(node->shader)
    {
        Cyb_FreeObject((Cyb_Object**)&node->shader);
    }
}


Cyb_Shader *Cyb_LoadShaderRW(Cyb_Renderer *renderer, SDL_RWops *file, 
    int doClose, const char *id)
{
    //Ignore this if the file is NULL
    if(!file)
    {
        return NULL;
    }

    //Init shader cache upon first use
    if(!shaderCache)
    {
        shaderCache = Cyb_CreateList(sizeof(Cyb_ShaderCacheNode), 
            (Cyb_FreeNodeProc)&Cyb_FreeShaderCacheNode);
            
        if(!shaderCache)
        {
            if(doClose)
            {
                SDL_RWclose(file);
            }
        
            return NULL;
        }
        
        //atexit(&Cyb_FreeShaderCache);
    }
    
    //Return cached shader if it has already been loaded
    for(Cyb_ShaderCacheNode *node = (Cyb_ShaderCacheNode*)shaderCache->first;
        node; node = (Cyb_ShaderCacheNode*)node->base.next)
    {
        //Is this the requested shader?
        if(node->shader->renderer == renderer && strcmp(node->id, id) == 0)
        {
            return (Cyb_Shader*)Cyb_NewObjectRef((Cyb_Object*)node->shader);
        }
    }
    
    //Load the file contents
    size_t size;
    char *data = SDL_LoadFile_RW(file, &size, doClose);
    
    if(!data)
    {
        return NULL;
    }
    
    //Allocate new shader
    Cyb_Shader *shader = (Cyb_Shader*)Cyb_CreateObject(sizeof(Cyb_Shader),
        (Cyb_FreeProc)&Cyb_FreeShader, CYB_SHADER);
        
    if(!shader)
    {
        return NULL;
    }
    
    //Initialize the shader
    shader->renderer = renderer;
    shader->program = 0;
    
    //Create shader program
    Cyb_SelectRenderer(renderer);
    Cyb_GLExtAPI *glExtAPI = Cyb_GetGLExtAPI(renderer);
    GLuint prog = glExtAPI->CreateProgram();
    
    if(!prog)
    {
        Cyb_FreeObject((Cyb_Object**)&shader);
        return NULL;
    }
    
    //Compile geometry shader
    char *source = strstr(data, "//Geometry Shader");
    char *end = strstr(data, "//End Geometry Shader");
    GLuint gShader = 0;
    
    if(source && end)
    {
        int len = (GLsizei)(end - source);
        gShader = glExtAPI->CreateShader(GL_GEOMETRY_SHADER);
    
        if(!gShader)
        {
            glExtAPI->DeleteProgram(prog);
            Cyb_FreeObject((Cyb_Object**)&shader);
            return NULL;
        }
    
        glExtAPI->ShaderSource(gShader, 1, (const char * const*)&source, &len);
        glExtAPI->CompileShader(gShader);
        int isCompiled;
        glExtAPI->GetShaderiv(gShader, GL_COMPILE_STATUS, &isCompiled);
    
        if(!isCompiled)
        {
            glExtAPI->GetShaderInfoLog(gShader, sizeof(infoLog), NULL, infoLog);
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                "[CybRender] Failed to compile geometry shader '%s': %s", 
                id, infoLog);
            glExtAPI->DeleteShader(gShader);
            glExtAPI->DeleteProgram(prog);
            Cyb_FreeObject((Cyb_Object**)&shader);
            return NULL;
        }
        
        glExtAPI->AttachShader(prog, gShader);
    }
    
    //Compile vertex shader
    source = strstr(data, "//Vertex Shader");
    end = strstr(data, "//End Vertex Shader");
    GLuint vShader = 0;
    
    if(source && end)
    {
        int len = (GLsizei)(end - source);
        vShader = glExtAPI->CreateShader(GL_VERTEX_SHADER);
    
        if(!vShader)
        {
            if(gShader)
            {
                glExtAPI->DeleteShader(gShader);
            }
            
            glExtAPI->DeleteProgram(prog);
            Cyb_FreeObject((Cyb_Object**)&shader);
            return NULL;
        }
    
        glExtAPI->ShaderSource(vShader, 1, (const char * const*)&source, &len);
        glExtAPI->CompileShader(vShader);
        int isCompiled;
        glExtAPI->GetShaderiv(vShader, GL_COMPILE_STATUS, &isCompiled);
    
        if(!isCompiled)
        {
            glExtAPI->GetShaderInfoLog(vShader, sizeof(infoLog), NULL, infoLog);
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                "[CybRender] Failed to compile vertex shader '%s': %s", 
                id, infoLog);
            
            if(gShader)
            {
                glExtAPI->DeleteShader(gShader);
            }
            
            glExtAPI->DeleteShader(vShader);
            glExtAPI->DeleteProgram(prog);
            Cyb_FreeObject((Cyb_Object**)&shader);
            return NULL;
        }
        
        glExtAPI->AttachShader(prog, vShader);
    }
    
    //Compile fragment shader
    source = strstr(data, "//Fragment Shader");
    end = strstr(data, "//End Fragment Shader");
    GLuint fShader = 0;
    
    if(source && end)
    {
        int len = (GLsizei)(end - source);
        fShader = glExtAPI->CreateShader(GL_FRAGMENT_SHADER);
    
        if(!vShader)
        {
            if(gShader)
            {
                glExtAPI->DeleteShader(gShader);
            }
            
            if(vShader)
            {
                glExtAPI->DeleteShader(vShader);
            }
            
            glExtAPI->DeleteProgram(prog);
            Cyb_FreeObject((Cyb_Object**)&shader);
            return NULL;
        }
    
        glExtAPI->ShaderSource(fShader, 1, (const char * const*)&source, &len);
        glExtAPI->CompileShader(fShader);
        int isCompiled;
        glExtAPI->GetShaderiv(fShader, GL_COMPILE_STATUS, &isCompiled);
    
        if(!isCompiled)
        {
            glExtAPI->GetShaderInfoLog(fShader, sizeof(infoLog), NULL, infoLog);
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                "[CybRender] Failed to compile fragment shader '%s': %s", 
                id, infoLog);
            
            if(gShader)
            {
                glExtAPI->DeleteShader(gShader);
            }
            
            if(vShader)
            {
                glExtAPI->DeleteShader(vShader);
            }
            
            glExtAPI->DeleteShader(fShader);
            glExtAPI->DeleteProgram(prog);
            Cyb_FreeObject((Cyb_Object**)&shader);
            return NULL;
        }
        
        glExtAPI->AttachShader(prog, fShader);
    }
    
    //Link shader program
    SDL_free(data);
    glExtAPI->LinkProgram(prog);
    int isLinked;
    glExtAPI->GetProgramiv(prog, GL_LINK_STATUS, &isLinked);
    
    if(!isLinked)
    {
        glExtAPI->GetProgramInfoLog(prog, sizeof(infoLog), NULL, infoLog);
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                "[CybRender] Failed to link shader program '%s': %s", 
                id, infoLog);
        
        if(gShader)
        {
            glExtAPI->DeleteShader(gShader);
        }
            
        if(vShader)
        {
            glExtAPI->DeleteShader(vShader);
        }
        
        if(fShader)
        {
            glExtAPI->DeleteShader(fShader);
        }
        
        glExtAPI->DeleteProgram(prog);
        Cyb_FreeObject((Cyb_Object**)&shader);
    }

    //Complete shader
    shader->program = prog;
    
    if(gShader)
    {
        glExtAPI->DeleteShader(gShader);
    }
        
    if(vShader)
    {
        glExtAPI->DeleteShader(vShader);
    }
    
    if(fShader)
    {
        glExtAPI->DeleteShader(fShader);
    }
    
    //Cache shader
    Cyb_ShaderCacheNode *node = (Cyb_ShaderCacheNode*)Cyb_InsertListElm(shaderCache,
        CYB_LIST_END);
        
    if(!node)
    {
        Cyb_FreeObject((Cyb_Object**)&shader);
        return NULL;
    }
    
    node->id = (char*)SDL_malloc(strlen(id) + 1);
    
    if(!node->id)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "[CybRender] Out of Memory");
        Cyb_RemoveListElm(shaderCache, CYB_LIST_END);
        Cyb_FreeObject((Cyb_Object**)&shader);
        return NULL;
    }
    
    strcpy(node->id, id);
    node->shader = (Cyb_Shader*)Cyb_NewObjectRef((Cyb_Object*)shader);
    SDL_Log("[CybRender] Shader '%s' loaded and cached.", id);
    return shader;
}


void Cyb_SelectShader(Cyb_Renderer *renderer, Cyb_Shader *shader)
{
    Cyb_SelectRenderer(renderer);
    Cyb_GetGLExtAPI(renderer)->UseProgram(shader->program);
}


void Cyb_SetMatrices(Cyb_Renderer *renderer, Cyb_Shader *shader,
    const char *name, int count, const Cyb_Mat4 *matrices)
{
    Cyb_GLExtAPI *glExtAPI = Cyb_GetGLExtAPI(renderer);
    GLint id = glExtAPI->GetUniformLocation(shader->program, name);
    glExtAPI->UniformMatrix4fv(id, count, FALSE, (const float*)matrices);
}
    

void Cyb_SetMatrix(Cyb_Renderer *renderer, Cyb_Shader *shader,
    const char *name, const Cyb_Mat4 *matrix)
{
    Cyb_SetMatrices(renderer, shader, name, 1, matrix);
}


void Cyb_SetTexture(Cyb_Renderer *renderer, Cyb_Shader *shader,
    const char *name, int texUnit)
{
    Cyb_GLExtAPI *glExtAPI = Cyb_GetGLExtAPI(renderer);
    GLint id = glExtAPI->GetUniformLocation(shader->program, name);
    glExtAPI->Uniform1i(id, texUnit);
}


void Cyb_SetVec3(Cyb_Renderer *renderer, Cyb_Shader *shader,
    const char *name, const Cyb_Vec3 *v)
{
    Cyb_GLExtAPI *glExtAPI = Cyb_GetGLExtAPI(renderer);
    GLint id = glExtAPI->GetUniformLocation(shader->program, name);
    glExtAPI->Uniform3fv(id, 1, (float*)v);
}


void Cyb_SetFloat(Cyb_Renderer *renderer, Cyb_Shader *shader,
    const char *name, float f)
{
    Cyb_GLExtAPI *glExtAPI = Cyb_GetGLExtAPI(renderer);
    GLint id = glExtAPI->GetUniformLocation(shader->program, name);
    glExtAPI->Uniform1f(id, f);
}