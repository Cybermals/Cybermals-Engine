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
Cyb_Mat4 mv;
Cyb_Mat4 mvp;

int drawTriangle = TRUE;
int drawCube = TRUE;
float angle = 0.0f;


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
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); //major version 3
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3); //minor version 3
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, TRUE);       //double-buffered
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);              //8-bits for red
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);            //8-bits for green
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);             //8-bits for blue
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 0);            //no alpha
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);           //24-bit dept buffer
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);          //8-bit stencil buffer
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
    
    {
        Cyb_Vec3 verts[] = {
            {-1.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f},
            {-1.0f, -1.0f, 1.0f},
            {1.0f, -1.0f, 1.0f},
            {-1.0f, 1.0f, -1.0f},
            {1.0f, 1.0f, -1.0f},
            {-1.0f, -1.0f, -1.0f},
            {1.0f, -1.0f, -1.0f}
        };
        
        Cyb_Vec3 norms[] = {
            {-1.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f},
            {-1.0f, -1.0f, 1.0f},
            {1.0f, -1.0f, 1.0f},
            {-1.0f, 1.0f, -1.0f},
            {1.0f, 1.0f, -1.0f},
            {-1.0f, -1.0f, -1.0f},
            {1.0f, -1.0f, -1.0f}
        };
        
        Cyb_Vec4 colors[] = {
            {0.0f, 0.0f, 0.0f, 1.0f},
            {1.0f, 0.0f, 0.0f, 1.0f},
            {0.0f, 1.0f, 0.0f, 1.0f},
            {1.0f, 1.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f, 1.0f},
            {1.0f, 0.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f, 1.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
        };
        
        unsigned int indices[] = {
            0, 2, 3,
            3, 1, 0,
            5, 7, 6,
            6, 4, 5,
            4, 6, 2,
            2, 0, 4,
            1, 3, 7,
            7, 5, 1,
            4, 0, 1,
            1, 5, 4,
            2, 6, 7,
            7, 3, 2
        };
        
        int vertCount = sizeof(verts) / sizeof(verts[0]);
        int indexCount = sizeof(indices) / sizeof(indices[0]);
        
        Cyb_UpdateMesh(renderer, cube, vertCount, verts, norms, colors, NULL,
            indexCount, indices);
    }
    
    //Initialize matrices
    Cyb_Translate(&v, 0.0f, 0.0f, -5.0f);
    Cyb_Perspective(&p, 45.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, .1f,
        1000.0f);
    //Cyb_Ortho(&p, -1.0f, 1.0f, 1.0f, -1.0f, .1f, 1000.0f);
    return 0;
}


void DrawTriangle(void)
{
    //Update model matrix
    Cyb_Rotate(&m, 0.0f, angle, 0.0f);
    
    //Enable depth testing and disable face culling
    glEnable(GL_DEPTH_TEST);
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


void DrawCube(void)
{
    //Update model matrix
    Cyb_Rotate(&m, 0.0f, -angle, 0.0f);
    
    //Enable depth testing and face culling
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    
    //Select shader
    Cyb_SelectShader(renderer, rainbowShader);
    
    //Set matrices
    Cyb_SetMatrix(renderer, rainbowShader, "m", &m);
    Cyb_SetMatrix(renderer, rainbowShader, "v", &v);
    Cyb_SetMatrix(renderer, rainbowShader, "p", &p);
    
    //Draw the cube
    Cyb_DrawMesh(renderer, cube);
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
        
        //Update angle
        angle += 1.0f;
        
        //Draw triangle?
        if(drawTriangle)
        {
            DrawTriangle();
        }
        
        //Draw cube?
        if(drawCube)
        {
            DrawCube();
        }
        
        //Swap buffers
        Cyb_RenderPresent(renderer);
        
        //Limit the framerate to 60 fps
        Cyb_NextFrame(FPS);
    }
    
    return 0;
}