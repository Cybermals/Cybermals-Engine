/*
CybUI - Timer API
*/

#include <SDL2/SDL.h>

#include "CybTimer.h"


//Globals
//=================================================================================
static int startTime = 0;
static int endTime = 0;


//Functions
//=================================================================================
void Cyb_NextFrame(int fps)
{
    //Update frame times
    startTime = endTime;
    endTime = SDL_GetTicks();
    int frameTime = endTime - startTime;
    
    //Limit framerate
    int targetTime = 1000 / fps;
    
    if(frameTime < targetTime)
    {
        SDL_Delay(targetTime - frameTime);
    }
}