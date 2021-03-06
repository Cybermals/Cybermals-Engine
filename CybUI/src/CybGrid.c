/*
CybUI - Grid API
*/

#include "CybGrid.h"
#include "CybWidgetList.h"


//Functions
//=================================================================================
void Cyb_FreeGrid(Cyb_Grid *grid)
{
    //Free the child widgets
    Cyb_FreeObject((Cyb_Object**)&grid->children);
}


Cyb_Grid *Cyb_CreateGrid(void)
{
    //Create a new grid
    Cyb_Grid *grid = (Cyb_Grid*)Cyb_CreateObject(sizeof(Cyb_Grid),
        (CybFreeProc)&Cyb_FreeGrid, CYB_GRID);
        
    if(!grid)
    {
        return NULL;
    }
    
    //Initialze the grid
    grid->draw = NULL;
    grid->parent = NULL;
    grid->children = Cyb_CreateWidgetList();
    grid->rows = 1;
    grid->cols = 1;
    grid->row = 0;
    grid->col = 0;
    grid->rowspan = 1;
    grid->colspan = 1;
    
    if(!grid->children)
    {
        Cyb_FreeObject((Cyb_Object**)&grid);
        return NULL;
    }
    
    return grid;
}


void Cyb_AddChildWidget(Cyb_Grid *parent, Cyb_Grid *child)
{}


void Cyb_DrawGrid(Cyb_Grid *grid, SDL_Renderer *renderer)
{}