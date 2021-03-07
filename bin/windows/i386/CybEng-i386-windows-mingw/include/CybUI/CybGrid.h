#ifndef CYBGRID_H
#define CYBGRID_H

/** @file
 * @brief CybUI - Grid API
 */

#include <SDL2/SDL.h>

#include "CybCommon.h"
#include "CybObjects.h"

 
#ifdef __cplusplus
extern "C" {
#endif

//Types
//=================================================================================
typedef struct Cyb_Grid Cyb_Grid;
typedef void (*CybDrawWidgetProc)(Cyb_Grid *grid, SDL_Renderer *renderer);
typedef void (*CybWidgetEventProc)(Cyb_Grid *grid, const SDL_Event *event);


//Structures
//=================================================================================
struct Cyb_Grid
{
    Cyb_Object base;
    CybDrawWidgetProc draw;
    CybWidgetEventProc handleEvent;
    Cyb_Grid *parent;
    Cyb_List *children;
    int rows;
    int cols;
    int row;
    int col;
    int rowspan;
    int colspan;
    int visible;
    SDL_Rect viewport;
    char *id;
    SDL_Texture *bg;
    Cyb_Object *data;
};


//Functions
//=================================================================================
CYBAPI void Cyb_FreeGridProc(Cyb_Grid *grid);
CYBAPI void Cyb_DrawGridProc(Cyb_Grid *grid, SDL_Renderer *renderer);
CYBAPI void Cyb_HandleGridEventProc(Cyb_Grid *grid, const SDL_Event *event);
CYBAPI Cyb_Grid *Cyb_CreateGrid(void);
CYBAPI void Cyb_AddChildGrid(Cyb_Grid *parent, Cyb_Grid *child);
CYBAPI void Cyb_SetGridID(Cyb_Grid *grid, const char *id);
CYBAPI Cyb_Grid *Cyb_GetGridByID(Cyb_Grid *grid, const char *id);
CYBAPI void Cyb_DrawUI(Cyb_Grid *grid, SDL_Renderer *renderer);
CYBAPI void Cyb_HandleUIEvent(Cyb_Grid *grid, const SDL_Event *event);


#ifdef __cplusplus
}
#endif

#endif