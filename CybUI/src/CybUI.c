/*
CybUI - Main API
*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "CybObjects.h"
#include "CybUI.h"
#include "CybUIEvents.h"


//Globals
//=================================================================================
static int refCnt = 0;


//Functions
//=================================================================================
int Cyb_InitUI(void)
{
    //Update ref count
    if(refCnt++ == 0)
    {
        //Initialize CybObjects
        if(Cyb_InitObjects())
        {
            refCnt = 0;
            return CYB_ERROR;
        }
        
        //Initialize UI loader
        if(Cyb_InitUILoader())
        {
            refCnt = 0;
            return CYB_ERROR;
        }
        
        //Register UI events
        if(Cyb_InitUIEvents())
        {
            refCnt = 0;
            return CYB_ERROR;
        }
        
        //Initialize SDL2 video subsystem
        SDL_Log("%s", "[CybUI] Initializing...");
        
        if(SDL_InitSubSystem(SDL_INIT_VIDEO) == -1)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[CybUI] %s",
                SDL_GetError());
            refCnt = 0;
            return CYB_ERROR;
        }
        
        atexit(&Cyb_FiniUI);
        
        //Initialize SDL2 event subsystem
        if(SDL_InitSubSystem(SDL_INIT_EVENTS) == -1)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[CybUI] %s",
                SDL_GetError());
            refCnt = 0;
            return CYB_ERROR;
        }
        
        //Initialize SDL2 timer subsystem
        if(SDL_InitSubSystem(SDL_INIT_TIMER) == -1)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[CybUI] %s",
                SDL_GetError());
            refCnt = 0;
            return CYB_ERROR;
        }
        
        //Initialize SDL2_image
        int flags = IMG_INIT_PNG | IMG_INIT_JPG;
        
        if(IMG_Init(flags) != flags)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[CybUI] %s",
                SDL_GetError());
            refCnt = 0;
            return CYB_ERROR;
        }
        
        //Initialize SDL2_ttf
        if(TTF_Init() == -1)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[CybUI] %s",
                SDL_GetError());
            return CYB_ERROR;
        }
    }
    
    return CYB_NO_ERROR;
}


void Cyb_FiniUI(void)
{
    //Update ref count
    if(--refCnt == 0)
    {
        //Quit SDL video subsystem and CybObjects
        SDL_Log("%s", "[CybUI] Shutting down...");
        SDL_QuitSubSystem(SDL_INIT_TIMER);
        SDL_QuitSubSystem(SDL_INIT_EVENTS);
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    }
}