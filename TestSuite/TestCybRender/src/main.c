/*
CybRender - Test Program
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>

#include "CybRender.h"
#include "CybUI.h"


//Macros
//===========================================================================
#define APP_TITLE         "CybRender Test"
#define WINDOW_X          SDL_WINDOWPOS_CENTERED
#define WINDOW_Y          SDL_WINDOWPOS_CENTERED
#define WINDOW_WIDTH      800
#define WINDOW_HEIGHT     600

#ifdef __ANDROID__
    #define WINDOW_FLAGS (SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL)
#else
    #define WINDOW_FLAGS (SDL_WINDOW_OPENGL)
#endif

#define FPS              60
#define BG_COLOR         0.0f, 0.5f, 1.0f, 1.0f


//Globals
//===========================================================================
SDL_Window *window = NULL;
Cyb_Renderer *renderer = NULL;

SDL_Surface *uiOverlay = NULL;
SDL_Renderer *uiRenderer = NULL;
Cyb_Grid *root = NULL;

Cyb_Shader *rainbowShader = NULL;

Cyb_Mesh *triangle = NULL;
Cyb_Mesh *cube = NULL;

Cyb_Mat4 m;
Cyb_Mat4 v;
Cyb_Mat4 p;

int drawTriangle = TRUE;
int drawCube = TRUE;


//Functions
//===========================================================================
void Quit(void)
{
    //Free root widget
    if(root)
    {
        Cyb_FreeObject((Cyb_Object**)&root);
    }
    
    //Free UI renderer
    if(uiRenderer)
    {
        SDL_DestroyRenderer(uiRenderer);
    }
    
    //Free UI overlay
    if(uiOverlay)
    {
        SDL_FreeSurface(uiOverlay);
    }
    
    //Free renderer (this also frees the resources associated with the renderer)
    if(renderer)
    {
        Cyb_FreeObject((Cyb_Object**)&renderer);
    }
    
    //Destroy window
    if(window)
    {
        SDL_DestroyWindow(window);
    }
}


int Init(void)
{
    //Initialize CybUI
    if(Cyb_InitUI())
    {
        return 1;
    }
    
    //Create a window
    window = SDL_CreateWindow(APP_TITLE, WINDOW_X, WINDOW_Y, WINDOW_WIDTH,
        WINDOW_HEIGHT, WINDOW_FLAGS);
        
    if(!window)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "Failed to create a window.");
        return 1;
    }
    
    atexit(&Quit);
    
    //Create renderer
    renderer = Cyb_CreateRenderer(window);
    
    if(!renderer)
    {
        return 1;
    }
    
    Cyb_SetRenderBGColor(renderer, BG_COLOR);
    
    //Create UI overlay
    uiOverlay = SDL_CreateRGBSurface(0, WINDOW_WIDTH, WINDOW_HEIGHT, 32,
        0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
        
    if(!uiOverlay)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s",
            "Failed to create UI overlay.");
        return 1;
    }
    
    //Create UI renderer
    uiRenderer = SDL_CreateSoftwareRenderer(uiOverlay);
    
    if(!uiRenderer)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s",
            "Failed to create UI renderer.");
        return 1;
    }
    
    //Load UI
    root = Cyb_LoadUI(uiRenderer, "data/UI/RendererUI.xml");
    
    if(!root)
    {
        return 1;
    }
    
    //Load Shaders
    rainbowShader = Cyb_LoadShader(renderer, "data/shaders/rainbow.glsl");
    
    if(!rainbowShader)
    {
        return 1;
    }
    
    //Create meshes
    triangle = Cyb_CreateMesh(renderer);
    
    if(!triangle)
    {
        return 1;
    }
    
    {
        Cyb_Vec3 verts[] = {
            {0.0f, 1.0f, 0.0f},
            {-1.0f, -1.0f, 0.0f},
            {1.0f, -1.0f, 0.0f}
        };
        
        Cyb_Vec3 norms[] = {
            {0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f}
        };
        
        Cyb_Vec4 colors[] = {
            {1.0f, 0.0f, 0.0f, 1.0f},
            {0.0f, 1.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f, 1.0f}
        };
        
        unsigned int indices[] = {
            0, 1, 2
        };
        
        int vertCount = sizeof(verts) / sizeof(verts[0]);
        int indexCount = sizeof(indices) / sizeof(indices[0]);
        
        Cyb_UpdateMesh(renderer, triangle, vertCount, verts, norms, colors, NULL,
            indexCount, indices);
    }
    
    cube = Cyb_CreateMesh(renderer);

    if(!cube)
    {
        return 1;
    }
    
    /* {
        Cyb_Vec3 verts[] = {};
        
        Cyb_Vec3 norms[] = {};
        
        Cyb_Vec4 colors[] = {};
        
        unsigned int indices = {};
        
        int vertCount = sizeof(verts) / sizeof(verts[0]);
        int indexCount = sizeof(indices) / sizeof(indices[0]);
    } */
    
    //Initialize matrices
    Cyb_Identity(&m);
    Cyb_Identity(&v);
    //Cyb_Translate(&v, 0.0f, 0.0f, -5.0f);
    Cyb_Identity(&p);
    //Cyb_Perspective(&p, 45.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, .1f,
    //    1000.0f);
    //Cyb_Ortho(&p, -1.0f, 1.0f, 1.0f, -1.0f, .1f, 1000.0f);
    return 0;
}


void DrawTriangle(void)
{
    //Update model matrix
    Cyb_Identity(&m);
    
    //Disable depth testing and face culling
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    
    //Select shader
    Cyb_SelectShader(renderer, rainbowShader);
    
    //Set matrices
    Cyb_SetMatrix(renderer, rainbowShader, "m", &m);
    Cyb_SetMatrix(renderer, rainbowShader, "v", &v);
    Cyb_SetMatrix(renderer, rainbowShader, "p", &p);
    
    //Draw the triangle
    Cyb_DrawMesh(renderer, triangle);
}


//Entry Point
//===========================================================================
int main(int argc, char **argv)
{
    //Initialize
    if(Init())
    {
        return 1;
    }
    
    //Main Loop
    while(TRUE)
    {
        //Process events
        SDL_Event event;
        
        while(SDL_PollEvent(&event))
        {
            //Quit event?
            if(event.type == SDL_QUIT)
            {
                return 0;
            }
        }
        
        //Clear the window
        Cyb_RenderClear(renderer);
        
        //Draw the UI
        //Cyb_DrawUI(root, uiRenderer);
        
        //Draw triangle?
        if(drawTriangle)
        {
            DrawTriangle();
        }
        
        //Swap buffers
        Cyb_RenderPresent(renderer);
        
        //Limit the framerate to 60 fps
        Cyb_NextFrame(FPS);
    }
    
    return 0;
}