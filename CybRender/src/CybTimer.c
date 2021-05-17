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


unsigned int Cyb_GetDeltaTime(void)
{
    #ifdef __ANDROID__
    //Note: On Android, the usual delta time calulations fail for unknown reasons.
    //Therefore, we will instead return a constant value that is equal to the delta
    //time of a perfect 30 FPS frame.
    return 1000 / 30;
    #else
    return SDL_GetTicks() - startTime;
    #endif
}