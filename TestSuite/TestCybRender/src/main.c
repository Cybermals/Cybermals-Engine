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
SDL_Surface *uiOverlay = NULL;
SDL_Renderer *uiRenderer = NULL;
Cyb_Grid *root = NULL;
Cyb_Renderer *renderer = NULL;
Cyb_Shader *rainbowShader = NULL;


//Functions
//===========================================================================
void Quit(void)
{
    //Free shaders
    if(rainbowShader)
    {
        Cyb_FreeObject((Cyb_Object**)&rainbowShader);
    }
    
    //Free renderer
    if(renderer)
    {
        Cyb_FreeObject((Cyb_Object**)&renderer);
    }
    
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
    
    //Create renderer
    renderer = Cyb_CreateRenderer(window);
    
    if(!renderer)
    {
        return 1;
    }
    
    Cyb_SetRenderBGColor(renderer, BG_COLOR);
    
    //Load Shaders
    rainbowShader = Cyb_LoadShader(renderer, "data/shaders/rainbow.glsl");
    
    if(!rainbowShader)
    {
        return 1;
    }
    
    return 0;
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
        
        //Swap buffers
        Cyb_RenderPresent(renderer);
        
        //Limit the framerate to 60 fps
        Cyb_NextFrame(FPS);
    }
    
    return 0;
}