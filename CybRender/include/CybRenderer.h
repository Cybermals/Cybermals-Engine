#ifndef CYBRENDERER_H
#define CYBRENDERER_H

/** @file
 * @brief CybRender - Renderer API
 */
 
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
 
#include "CybCommon.h"

 
#ifdef __cplusplus
extern "C" {
#endif

//Types
//=================================================================================
typedef struct Cyb_Renderer Cyb_Renderer;


//Functions
//=================================================================================
CYBAPI Cyb_Renderer *Cyb_CreateRenderer(SDL_Window *window);
CYBAPI void Cyb_SetRenderBGColor(Cyb_Renderer *renderer, float r, float g, float b,
    float a);
CYBAPI void Cyb_RenderClear(Cyb_Renderer *renderer);
CYBAPI void Cyb_RenderPresent(Cyb_Renderer *renderer);


#ifdef __cplusplus
}
#endif

#endif