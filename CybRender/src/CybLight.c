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
    light->pos.x = 0.0f;
    light->pos.y = 0.0f;
    light->pos.z = 0.0f;
    light->ambient.x = .2f;
    light->ambient.y = .2f;
    light->ambient.z = .2f;
    light->diffuse.x = .5f;
    light->diffuse.y = .5f;
    light->diffuse.z = .5f;
    light->specular.x = 1.0f;
    light->specular.y = 1.0f;
    light->specular.z = 1.0f;
    return light;
}