/*
CybRender - Renderer API
*/

#include <SDL2/SDL_opengl.h>

#include "CybObjects.h"
#include "CybRenderer.h"


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
    glExtAPI->CreateShader = SDL_GL_GetProcAddress("glCreateShader");
    glExtAPI->DeleteShader = SDL_GL_GetProcAddress("glDeleteShader");
    glExtAPI->ShaderSource = SDL_GL_GetProcAddress("glShaderSource");
    glExtAPI->CompileShader = SDL_GL_GetProcAddress("glCompileShader");
    glExtAPI->GetShaderiv = SDL_GL_GetProcAddress("glGetShaderiv");
    glExtAPI->GetShaderInfoLog = SDL_GL_GetProcAddress("glGetShaderInfoLog");
    
    //Import program functions
    glExtAPI->CreateProgram = SDL_GL_GetProcAddress("glCreateProgram");
    glExtAPI->DeleteProgram = SDL_GL_GetProcAddress("glDeleteProgram");
    glExtAPI->AttachShader = SDL_GL_GetProcAddress("glAttachShader");
    glExtAPI->LinkProgram = SDL_GL_GetProcAddress("glLinkProgram");
    glExtAPI->GetProgramiv = SDL_GL_GetProcAddress("glGetProgramiv");
    glExtAPI->GetProgramInfoLog = SDL_GL_GetProcAddress("glGetProgramInfoLog");
    glExtAPI->ValidateProgram = SDL_GL_GetProcAddress("glValidateProgram");
    glExtAPI->UseProgram = SDL_GL_GetProcAddress("glUseProgram");
    glExtAPI->GetUniformLocation = SDL_GL_GetProcAddress("glGetUniformLocation");
    glExtAPI->UniformMatrix4fv = SDL_GL_GetProcAddress("glUniformMatrix4fv");
    
    //Import buffer functions
    glExtAPI->GenBuffers = SDL_GL_GetProcAddress("glGenBuffers");
    glExtAPI->DeleteBuffers = SDL_GL_GetProcAddress("glDeleteBuffers");
    glExtAPI->BindBuffer = SDL_GL_GetProcAddress("glBindBuffer");
    glExtAPI->BufferData = SDL_GL_GetProcAddress("glBufferData");
    glExtAPI->MapBuffer = SDL_GL_GetProcAddress("glMapBuffer");
    glExtAPI->UnmapBuffer = SDL_GL_GetProcAddress("glUnmapBuffer");
    
    //Import vertex array functions
    glExtAPI->GenVertexArrays = SDL_GL_GetProcAddress("glGenVertexArrays");
    glExtAPI->DeleteVertexArrays = SDL_GL_GetProcAddress("glDeleteVertexArrays");
    glExtAPI->BindVertexArray = SDL_GL_GetProcAddress("glBindVertexArray");
    
    //Import vertex attrib functions
    glExtAPI->EnableVertexAttribArray = SDL_GL_GetProcAddress(
        "glEnableVertexAttribArray");
    glExtAPI->DisableVertexAttribArray = SDL_GL_GetProcAddress(
        "glDisableVertexAttribArray");
    glExtAPI->VertexAttribPointer = SDL_GL_GetProcAddress("glVertexAttribPointer");
    
    //Verify that all functions were imported
    if(!glExtAPI->CreateShader ||
        !glExtAPI->DeleteShader ||
        !glExtAPI->ShaderSource ||
        !glExtAPI->CompileShader ||
        !glExtAPI->GetShaderiv ||
        !glExtAPI->GetShaderInfoLog ||
        !glExtAPI->CreateProgram ||
        !glExtAPI->DeleteProgram ||
        !glExtAPI->AttachShader ||
        !glExtAPI->LinkProgram ||
        !glExtAPI->GetProgramiv ||
        !glExtAPI->GetProgramInfoLog ||
        !glExtAPI->ValidateProgram ||
        !glExtAPI->UseProgram ||
        !glExtAPI->GetUniformLocation ||
        !glExtAPI->UniformMatrix4fv ||
        !glExtAPI->GenBuffers ||
        !glExtAPI->DeleteBuffers ||
        !glExtAPI->BindBuffer ||
        !glExtAPI->BufferData ||
        !glExtAPI->MapBuffer ||
        !glExtAPI->UnmapBuffer ||
        !glExtAPI->GenVertexArrays ||
        !glExtAPI->DeleteVertexArrays ||
        !glExtAPI->BindVertexArray ||
        !glExtAPI->EnableVertexAttribArray ||
        !glExtAPI->VertexAttribPointer)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s",
            "[CybRender] Failed to import a required OpenGL function. Please update your graphics drivers and try again.");
        return CYB_ERROR;
    }
    
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
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, TRUE);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 0);
    renderer->glCtx = SDL_GL_CreateContext(window);
    
    if(!renderer->glCtx)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "[CybRender] Failed to create OpenGL context.");
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