/*
CybUI - Main API
*/

#include <SDL2/SDL.h>

#include "CybObjects.h"
#include "CybUI.h"


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
            return CYB_ERROR;
        }
        
        //Initialize SDL video subsystem
        SDL_Log("%s", "[CybUI] Initializing...");
        
        if(SDL_InitSubSystem(SDL_INIT_VIDEO) == -1)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[CybUI] %s",
                SDL_GetError());
            return CYB_ERROR;
        }
        
        atexit(&Cyb_FiniUI);
        
        if(SDL_InitSubSystem(SDL_INIT_EVENTS) == -1)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[CybUI] %s",
                SDL_GetError());
            return CYB_ERROR;
        }
        
        if(SDL_InitSubSystem(SDL_INIT_TIMER) == -1)
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