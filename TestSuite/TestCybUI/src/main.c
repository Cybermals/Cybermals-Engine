/*
CybUI - Test Program
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>

#include <CybUI.h>


//Macros
//===========================================================================
#define APP_TITLE        "CybUI Test"
#define WINDOW_X         SDL_WINDOWPOS_CENTERED
#define WINDOW_Y         SDL_WINDOWPOS_CENTERED
#define WINDOW_WIDTH     800
#define WINDOW_HEIGHT    600

#ifdef __android__
    #define WINDOW_FLAGS SDL_WINDOW_FULLSCREEN
#else
    #define WINDOW_FLAGS 0
#endif

#define FPS              60


//Globals
//===========================================================================
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;


//Entry Point
//===========================================================================
int main(int argc, char **argv)
{
    //Init CybUI
    if(Cyb_InitUI())
    {
        return 1;
    }
    
    //Create a window and renderer
    SDL_Log("%s", "Creating a window...");
    
    if(SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_FLAGS,
        &window, &renderer) == -1)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "Failed to create a window.");
        return 1;
    }
    
    SDL_SetWindowTitle(window, APP_TITLE);
    SDL_SetWindowPosition(window, WINDOW_X, WINDOW_Y);
    
    //Init UI
    SDL_Log("%s", "Loading UI...");
    Cyb_Grid *root = Cyb_LoadUI("data/UI/UI.xml");
    
    if(!root)
    {
    }
    
    //Main Loop
    while(TRUE)
    {
        //Process events
        SDL_Event event;
        
        while(SDL_PollEvent(&event))
        {
            //Process next event
            switch(event.type)
            {
                //Quit Event
            case SDL_QUIT:
                return 0;
            }
        }
        
        //Limit framerate
        Cyb_NextFrame(FPS);
    }
    
    return 0;
}