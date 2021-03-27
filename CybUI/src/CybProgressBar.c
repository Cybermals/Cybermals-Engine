/*
CybUI - Progress Bar API
*/

#include "CybProgressBar.h"
#include "CybWidgetList.h"


//Functions
//==================================================================================
Cyb_Grid *Cyb_CreateProgressBar(void)
{
    //Allocate new progress bar
    Cyb_Grid *progBar = Cyb_CreateGrid();
    
    if(!progBar)
    {
        return NULL;
    }
    
    //Initialize progress bar
    progBar->base.type = CYB_PROGRESSBAR;
    Cyb_Grid *bar = Cyb_CreateGrid();
    
    if(!bar)
    {
        Cyb_FreeObject((Cyb_Object**)&progBar);
        return NULL;
    }
    
    Cyb_AddChildGrid(progBar, bar);
    return progBar;
}


void Cyb_SetProgressTextures(Cyb_Grid *progBar, SDL_Texture *fg,
    SDL_Texture *bg)
{
    progBar->bg = bg;
    Cyb_WidgetNode *node = (Cyb_WidgetNode*)Cyb_GetListElm(progBar->children, 0);
    
    if(!node)
    {
        return;
    }
    
    node->widget->bg = fg;
}


void Cyb_SetMaxProgressValue(Cyb_Grid *progBar, int max)
{
    progBar->cols = max;
}


void Cyb_SetProgressValue(Cyb_Grid *progBar, int value)
{
    Cyb_WidgetNode *node = (Cyb_WidgetNode*)Cyb_GetListElm(progBar->children, 0);
    
    if(!node)
    {
        return;
    }
    
    node->widget->colspan = value;
}