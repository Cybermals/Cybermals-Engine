/*
CybRender - Light API
*/

#include <SDL2/SDL.h>

#include "CybLight.h"


//Functions
//=================================================================================
Cyb_Light *Cyb_CreateLight(void)
{
    //Allocate a new light
    Cyb_Light *light = (Cyb_Light*)Cyb_CreateObject(sizeof(Cyb_Light), NULL,
        CYB_LIGHT);
        
    if(!light)
    {
        return NULL;
    }
    
    //Initialize the light
    light->color.x = 1.0f;
    light->color.y = 1.0f;
    light->color.z = 1.0f;
    light->ambientStrength = .1f;
    light->specularStrength = .5f;
    return light;
}