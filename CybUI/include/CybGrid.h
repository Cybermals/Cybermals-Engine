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


//Structures
//=================================================================================
struct Cyb_Grid
{
    Cyb_Object base;
    CybDrawWidgetProc draw;
    Cyb_Grid *parent;
    Cyb_List *children;
    int rows;
    int cols;
    int row;
    int col;
    int rowspan;
    int colspan;
};


//Functions
//=================================================================================
CYBAPI void Cyb_FreeGrid(Cyb_Grid *grid);
CYBAPI Cyb_Grid *Cyb_CreateGrid(void);
CYBAPI void Cyb_AddChildWidget(Cyb_Grid *parent, Cyb_Grid *child);
CYBAPI void Cyb_DrawGrid(Cyb_Grid *grid, SDL_Renderer *renderer);


#ifdef __cplusplus
}
#endif

#endif