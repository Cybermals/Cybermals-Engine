/*
CybUI - Timer API
*/

#include <SDL2/SDL.h>

#include "CybTimer.h"


//Globals
//=================================================================================
static unsigned int startTime = 0;
static unsigned int endTime = 0;


//Functions
//=================================================================================
void Cyb_NextFrame(int fps)
{
    //Update frame times
    startTime = endTime;
    endTime = SDL_GetTicks();
    unsigned int frameTime = endTime - startTime;
    
    //Limit framerate
    unsigned int targetTime = 1000 / fps;
    
    if(frameTime < targetTime)
    {
        SDL_Delay(targetTime - frameTime);
    }
}