/*
CybUI - UI Events API
*/

#include <SDL2/SDL.h>

#include "CybUIEvents.h"


//Globals
//=================================================================================
static int isInit = FALSE;

int CYB_UI_MOUSEMOTION = -1;
int CYB_UI_MOUSEBUTTONDOWN = -1;
int CYB_UI_MOUSEBUTTONUP = -1;


//Functions
//=================================================================================
int Cyb_InitUIEvents(void)
{
    //Only initialize this once
    if(isInit)
    {
        return CYB_NO_ERROR;
    }
    
    //Initialize UI events
    CYB_UI_MOUSEMOTION = SDL_RegisterEvents(3);
    CYB_UI_MOUSEBUTTONDOWN = CYB_UI_MOUSEMOTION + 1;
    CYB_UI_MOUSEBUTTONUP = CYB_UI_MOUSEMOTION + 2;
    
    if(CYB_UI_MOUSEMOTION == -1)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "[CybUI] Failed to register UI events.");
        return CYB_ERROR;
    }
    
    isInit = TRUE;
    return CYB_NO_ERROR;
}