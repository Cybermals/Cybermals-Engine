/*
CybRender - Renderer API
*/

#include <SDL2/SDL_opengl.h>

#include "CybObjects.h"
#include "CybRenderer.h"

#define IMPORT_GL_FUNC(var, type, name) var = (type)SDL_GL_GetProcAddress(name); \
    \
    if(!var) \
    { \
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, \
            "[CybRender] Failed to import '%s'. Please update your graphics drivers.", \
            name); \
        return CYB_ERROR; \
    }


//Structures
//=================================================================================
struct Cyb_Renderer
{
    Cyb_Object base;
    SDL_Window *window;
    SDL_GLContext glCtx;
    Cyb_GLExtAPI glExtAPI;
};


//Globals
//=================================================================================
static Cyb_Renderer *current = NULL;


//Functions
//=================================================================================
static void Cyb_FreeRenderer(Cyb_Renderer *renderer)
{
    //Ensure that this renderer is not current
    if(current == renderer)
    {
        current = NULL;
        SDL_GL_MakeCurrent(renderer->window, NULL);
    }
    
    //Free OpenGL context
    if(renderer->glCtx)
    {
        SDL_GL_DeleteContext(renderer->glCtx);
    }
    
    //Finalize CybObjects
    Cyb_FiniObjects();
}


static int Cyb_InitGLExtAPI(Cyb_GLExtAPI *glExtAPI)
{
    //Import shader functions
    IMPORT_GL_FUNC(
        glExtAPI->CreateShader, 
        PFNGLCREATESHADERPROC, 
        "glCreateShader"
    );
    IMPORT_GL_FUNC(
        glExtAPI->DeleteShader, 
        PFNGLDELETESHADERPROC, 
        "glDeleteShader"
    );
    IMPORT_GL_FUNC(
        glExtAPI->ShaderSource, 
        PFNGLSHADERSOURCEPROC, 
        "glShaderSource"
    );
    IMPORT_GL_FUNC(
        glExtAPI->CompileShader, 
        PFNGLCOMPILESHADERPROC, 
        "glCompileShader"
    );
    IMPORT_GL_FUNC(
        glExtAPI->GetShaderiv, 
        PFNGLGETSHADERIVPROC, 
        "glGetShaderiv"
    );
    IMPORT_GL_FUNC(
        glExtAPI->GetShaderInfoLog, 
        PFNGLGETSHADERINFOLOGPROC,
        "glGetShaderInfoLog"
    );
    
    //Import program functions
    IMPORT_GL_FUNC(
        glExtAPI->CreateProgram, 
        PFNGLCREATEPROGRAMPROC, 
        "glCreateProgram"
    );
    IMPORT_GL_FUNC(
        glExtAPI->DeleteProgram, 
        PFNGLDELETEPROGRAMPROC,
        "glDeleteProgram"
    );
    IMPORT_GL_FUNC(
        glExtAPI->AttachShader, 
        PFNGLATTACHSHADERPROC,
        "glAttachShader"
    );
    IMPORT_GL_FUNC(
        glExtAPI->BindAttribLocation,
        PFNGLBINDATTRIBLOCATIONPROC,
        "glBindAttribLocation"
    );
    IMPORT_GL_FUNC(
        glExtAPI->LinkProgram, 
        PFNGLLINKPROGRAMPROC,
        "glLinkProgram"
    );
    IMPORT_GL_FUNC(
        glExtAPI->GetProgramiv, 
        PFNGLGETPROGRAMIVPROC,
        "glGetProgramiv"
    );
    IMPORT_GL_FUNC(
        glExtAPI->GetProgramInfoLog, 
        PFNGLGETPROGRAMINFOLOGPROC,
        "glGetProgramInfoLog"
    );
    IMPORT_GL_FUNC(
        glExtAPI->ValidateProgram, 
        PFNGLVALIDATEPROGRAMPROC,
        "glValidateProgram"
    );
    IMPORT_GL_FUNC(
        glExtAPI->UseProgram, 
        PFNGLUSEPROGRAMPROC,
        "glUseProgram"
    );
    IMPORT_GL_FUNC(
        glExtAPI->GetUniformLocation, 
        PFNGLGETUNIFORMLOCATIONPROC,
        "glGetUniformLocation"
    );
    IMPORT_GL_FUNC(
        glExtAPI->UniformMatrix4fv, 
        PFNGLUNIFORMMATRIX4FVPROC,
        "glUniformMatrix4fv"
    );
    IMPORT_GL_FUNC(
        glExtAPI->Uniform1i, 
        PFNGLUNIFORM1IPROC,
        "glUniform1i"
    );
    IMPORT_GL_FUNC(
        glExtAPI->Uniform3fv, 
        PFNGLUNIFORM3FVPROC,
        "glUniform3fv"
    );
    IMPORT_GL_FUNC(
        glExtAPI->Uniform1f, 
        PFNGLUNIFORM1FPROC,
        "glUniform1f"
    );
    
    //Import buffer functions
    IMPORT_GL_FUNC(
        glExtAPI->GenBuffers, 
        PFNGLGENBUFFERSPROC,
        "glGenBuffers"
    );
    IMPORT_GL_FUNC(
        glExtAPI->DeleteBuffers, 
        PFNGLDELETEBUFFERSPROC,
        "glDeleteBuffers"
    );
    IMPORT_GL_FUNC(
        glExtAPI->BindBuffer, 
        PFNGLBINDBUFFERPROC,
        "glBindBuffer"
    );
    IMPORT_GL_FUNC(
        glExtAPI->BufferData, 
        PFNGLBUFFERDATAPROC,
        "glBufferData"
    );
    
    //Import vertex attrib functions
    IMPORT_GL_FUNC(
        glExtAPI->EnableVertexAttribArray, 
        PFNGLENABLEVERTEXATTRIBARRAYPROC,
        "glEnableVertexAttribArray"
    );
    IMPORT_GL_FUNC(
        glExtAPI->DisableVertexAttribArray,
        PFNGLDISABLEVERTEXATTRIBARRAYPROC,
        "glDisableVertexAttribArray"
    );
    IMPORT_GL_FUNC(
        glExtAPI->VertexAttribPointer,
        PFNGLVERTEXATTRIBPOINTERPROC,
        "glVertexAttribPointer"
    );
    
    //Import texture functions
    IMPORT_GL_FUNC(
        glExtAPI->ActiveTexture,
        PFNGLACTIVETEXTUREPROC,
        "glActiveTexture"
    );
    
    /* //Import instanced drawing functions
    IMPORT_GL_FUNC(
        glExtAPI->DrawElementsInstanced,
        PFNGLDRAWELEMENTSINSTANCEDPROC,
        "glDrawElementsInstanced"
    );
    IMPORT_GL_FUNC(
        glExtAPI->VertexAttribDivisor,
        PFNGLVERTEXATTRIBDIVISORPROC,
        "glVertexAttribDivisor"
    ); */
    
    return CYB_NO_ERROR;
}


Cyb_Renderer *Cyb_CreateRenderer(SDL_Window *window)
{
    //Initialize CybObjects
    if(Cyb_InitObjects())
    {
        return NULL;
    }
    
    //Allocate new renderer
    Cyb_Renderer *renderer = (Cyb_Renderer*)Cyb_CreateObject(sizeof(Cyb_Renderer),
        (Cyb_FreeProc)&Cyb_FreeRenderer, CYB_RENDERER);
        
    if(!renderer)
    {
        return NULL;
    }
    
    //Initialize the renderer
    renderer->window = window;
    renderer->glCtx = SDL_GL_CreateContext(window);
    
    if(!renderer->glCtx)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[CybRender] %s",
            SDL_GetError());
        Cyb_FreeObject((Cyb_Object**)&renderer);
        return NULL;
    }
    
    SDL_GL_MakeCurrent(renderer->window, renderer->glCtx);
    
    if(Cyb_InitGLExtAPI(&renderer->glExtAPI))
    {
        Cyb_FreeObject((Cyb_Object**)&renderer);
        return NULL;
    }

    return renderer;
}


void Cyb_SelectRenderer(Cyb_Renderer *renderer)
{
    //Ensure that this renderer is current
    if(current != renderer)
    {
        //Make the renderer current
        SDL_GL_MakeCurrent(renderer->window, renderer->glCtx);
        current = renderer;
    }
}


void Cyb_SetRenderBGColor(Cyb_Renderer *renderer, float r, float g, float b,
    float a)
{
    //Set the clear color
    Cyb_SelectRenderer(renderer);
    glClearColor(r, g, b, a);
}


void Cyb_RenderClear(Cyb_Renderer *renderer)
{
    //Clear the color and depth buffers
    Cyb_SelectRenderer(renderer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


Cyb_GLExtAPI *Cyb_GetGLExtAPI(Cyb_Renderer *renderer)
{
    return &renderer->glExtAPI;
}


void Cyb_RenderPresent(Cyb_Renderer *renderer)
{
    SDL_GL_SwapWindow(renderer->window);
}