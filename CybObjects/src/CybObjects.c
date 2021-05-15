/*
CybObjects - Main API
*/

#include <stdlib.h>

#include <SDL2/SDL.h>

#include "CybObjects.h"


//Globals
//==================================================================================
static int refCnt = 0;


//Functions
//==================================================================================
float Cyb_GetVersion(void)
{
    return CYB_VERSION;
}


const char *Cyb_GetVersionString(void)
{
    return CYB_VERSION_STRING;
}


int Cyb_InitObjects(void)
{
    //Update ref count
    if(refCnt++ == 0)
    {        
        //Initialize SDL2 threading support
        SDL_Log("%s", "[CybObjects] Initializing...");
    
        if(SDL_Init(0) == -1)
        {
            refCnt--;
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[CybObjects] %s",
                SDL_GetError());
            return CYB_ERROR;
        }
    
        atexit(&Cyb_FiniObjects);
    }
    
    return CYB_NO_ERROR;
}


void Cyb_FiniObjects(void)
{
    //Update ref count
    if(--refCnt == 0)
    {
        //Uninitialize SDL2
        SDL_Log("%s", "[CybObjects] Shutting down...");
        SDL_Quit();
    }
}