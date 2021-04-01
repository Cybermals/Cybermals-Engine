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
};


//Globals
//=================================================================================
static Cyb_Renderer *current = NULL;


//Functions
//=================================================================================
static void Cyb_FreeRenderer(Cyb_Renderer *renderer)
{
    //Free OpenGL context
    if(renderer->glCtx)
    {
        SDL_GL_DeleteContext(renderer->glCtx);
    }
}


Cyb_Renderer *Cyb_CreateRenderer(SDL_Window *window)
{
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
    return renderer;
}


void Cyb_SetRenderBGColor(Cyb_Renderer *renderer, float r, float g, float b,
    float a)
{
    //Ensure that this renderer is current
    if(current != renderer)
    {
        //Make the renderer current
        SDL_GL_MakeCurrent(renderer->window, renderer->glCtx);
        current = renderer;
    }
    
    //Set the clear color
    glClearColor(r, g, b, a);
}


void Cyb_RenderClear(Cyb_Renderer *renderer)
{
    //Ensure that this renderer is current
    if(current != renderer)
    {
        //Make the renderer current
        SDL_GL_MakeCurrent(renderer->window, renderer->glCtx);
        current = renderer;
    }
    
    //Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Cyb_RenderPresent(Cyb_Renderer *renderer)
{
    SDL_GL_SwapWindow(renderer->window);
}