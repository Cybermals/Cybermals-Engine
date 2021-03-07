/*
CybUI - Grid API
*/

#include <string.h>

#include "CybGrid.h"
#include "CybUIEvents.h"
#include "CybWidgetList.h"


//Functions
//=================================================================================
void Cyb_FreeGridProc(Cyb_Grid *grid)
{
    //Free the child widgets
    Cyb_FreeObject((Cyb_Object**)&grid->children);
    
    //Free the ID string
    if(grid->id)
    {
        SDL_free(grid->id);
    }
    
    //Free private data
    if(grid->data)
    {
        Cyb_FreeObject((Cyb_Object**)&grid->data);
    }
}


void Cyb_DrawGridProc(Cyb_Grid *grid, SDL_Renderer *renderer)
{
    //Do nothing if this widget has a BG
    if(grid->bg)
    {
        return;
    }
    
    //Calculate cell size
    SDL_Point cellSize;
    cellSize.x = grid->viewport.w / grid->cols;
    cellSize.y = grid->viewport.h / grid->rows;
    
    //Draw the cells
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    for(int y = 0; y < grid->rows; y++)
    {
        for(int x = 0; x < grid->cols; x++)
        {
            SDL_Rect rect;
            rect.x = x * cellSize.x;
            rect.y = y * cellSize.y;
            rect.w = cellSize.x;
            rect.h = cellSize.y;
            
            SDL_RenderDrawRect(renderer, &rect);
        }
    }
}


void Cyb_HandleGridEventProc(Cyb_Grid *grid, const SDL_Event *event)
{}


Cyb_Grid *Cyb_CreateGrid(void)
{
    //Create a new grid
    Cyb_Grid *grid = (Cyb_Grid*)Cyb_CreateObject(sizeof(Cyb_Grid),
        (CybFreeProc)&Cyb_FreeGridProc, CYB_GRID);
        
    if(!grid)
    {
        return NULL;
    }
    
    //Initialze the grid
    grid->draw = &Cyb_DrawGridProc;
    grid->handleEvent = &Cyb_HandleGridEventProc;
    grid->parent = NULL;
    grid->children = Cyb_CreateWidgetList();
    grid->rows = 1;
    grid->cols = 1;
    grid->row = 0;
    grid->col = 0;
    grid->rowspan = 1;
    grid->colspan = 1;
    grid->visible = TRUE;
    grid->id = NULL;
    grid->bg = NULL;
    grid->data = NULL;
    
    if(!grid->children)
    {
        Cyb_FreeObject((Cyb_Object**)&grid);
        return NULL;
    }
    
    return grid;
}


void Cyb_AddChildGrid(Cyb_Grid *parent, Cyb_Grid *child)
{
    //Ensure that the child is not NULL
    if(!child)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "[CybUI] Child widgets cannot be NULL.");
        return;
    }
    
    //Add the child widget
    Cyb_WidgetNode *node = (Cyb_WidgetNode*)Cyb_InsertListElm(parent->children,
        CYB_LIST_END);
        
    if(!node)
    {
        return;
    }
    
    node->widget = (Cyb_Grid*)Cyb_NewObjectRef((Cyb_Object*)child);
    child->parent = parent; //this is a weak ref on purpose
}


void Cyb_SetGridID(Cyb_Grid *grid, const char *id)
{
    //Free previous ID if there is already one
    if(grid->id)
    {
        SDL_free(grid->id);
    }

    //Store a copy of the ID string
    grid->id = (char*)SDL_malloc(strlen(id) + 1);
    
    if(!grid->id)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", "[CybUI] Out of Memory");
        return;
    }
    
    strcpy(grid->id, id);
}


Cyb_Grid *Cyb_GetGridByID(Cyb_Grid *grid, const char *id)
{
    //Does the ID match this grid?
    if(grid->id && strcmp(grid->id, id) == 0)
    {
        return grid;
    }
    
    //If not, recurse down and check the child widgets
    for(Cyb_WidgetNode *child = (Cyb_WidgetNode*)grid->children->first;
        child; child = (Cyb_WidgetNode*)child->base.next)
    {
        Cyb_Grid *grid = Cyb_GetGridByID(child->widget, id);
        
        if(grid)
        {
            return grid;
        }
    }
    
    //Not found
    return NULL;
}


void Cyb_DrawUI(Cyb_Grid *grid, SDL_Renderer *renderer)
{
    //Save current viewport
    SDL_Rect viewport;
    SDL_RenderGetViewport(renderer, &viewport);
    
    //Set local viewport
    SDL_Point cellSize;
    cellSize.x = viewport.w;
    cellSize.y = viewport.h;
    
    if(grid->parent)
    {
        cellSize.x /= grid->parent->cols;
        cellSize.y /= grid->parent->rows;
    }
    
    grid->viewport.x = viewport.x + cellSize.x * grid->col;
    grid->viewport.y = viewport.y + cellSize.y * grid->row;
    grid->viewport.w = cellSize.x * grid->colspan;
    grid->viewport.h = cellSize.y * grid->rowspan;
    
    SDL_RenderSetViewport(renderer, &grid->viewport);
    
    //Draw BG
    if(grid->bg)
    {
        SDL_RenderCopy(renderer, grid->bg, NULL, NULL);
    }
    
    //Call the draw procedure
    if(grid->draw && grid->visible)
    {
        grid->draw(grid, renderer);
    }
    
    //Draw child widgets
    for(Cyb_WidgetNode *child = (Cyb_WidgetNode*)grid->children->first;
        child; child = (Cyb_WidgetNode*)child->base.next)
    {
        Cyb_DrawUI(child->widget, renderer);
    }
    
    //Restore previous viewport
    SDL_RenderSetViewport(renderer, &viewport);
}


void Cyb_HandleUIEvent(Cyb_Grid *grid, const SDL_Event *event)
{
    //Process events that should be handled by all widgets
    SDL_Point mousePos;
    SDL_Event uiEvent;
    
    switch(event->type)
    {
        //Mouse Motion
    case SDL_MOUSEMOTION:
    {
        mousePos.x = event->motion.x;
        mousePos.y = event->motion.y;
        
        if(SDL_PointInRect(&mousePos, &grid->viewport))
        {
            uiEvent.user.type = CYB_UI_MOUSEMOTION;
            uiEvent.user.timestamp = SDL_GetTicks();
            uiEvent.user.windowID = event->motion.windowID;
            uiEvent.user.code = 0;     //unused for now
            uiEvent.user.data1 = grid; //widget pointer
            uiEvent.user.data2 = NULL; //unused for now
            
            SDL_PushEvent(&uiEvent);
        }
    
        break;
    }
        
        //Mouse Button Down
    case SDL_MOUSEBUTTONDOWN:
    {
        mousePos.x = event->button.x;
        mousePos.y = event->button.y;
        
        if(SDL_PointInRect(&mousePos, &grid->viewport))
        {
            uiEvent.user.type = CYB_UI_MOUSEBUTTONDOWN;
            uiEvent.user.timestamp = SDL_GetTicks();
            uiEvent.user.windowID = event->button.windowID;
            uiEvent.user.code = event->button.button; //button index
            uiEvent.user.data1 = grid;                //widget pointer
            uiEvent.user.data2 = NULL;                //unused for now
            
            SDL_PushEvent(&uiEvent);
        }
    
        break;
    }
        
        //Mouse Button Up
    case SDL_MOUSEBUTTONUP:
    {
        mousePos.x = event->button.x;
        mousePos.y = event->button.y;
        
        if(SDL_PointInRect(&mousePos, &grid->viewport))
        {
            uiEvent.user.type = CYB_UI_MOUSEBUTTONUP;
            uiEvent.user.timestamp = SDL_GetTicks();
            uiEvent.user.windowID = event->button.windowID;
            uiEvent.user.code = event->button.button; //button index
            uiEvent.user.data1 = grid;                //widget pointer
            uiEvent.user.data2 = NULL;                //unused for now
            
            SDL_PushEvent(&uiEvent);
        }
    
        break;
    }
    }
    
    //Call the event handler
    if(grid->handleEvent)
    {
        grid->handleEvent(grid, event);
    }
    
    //Propagate the event to child widgets
    for(Cyb_WidgetNode *child = (Cyb_WidgetNode*)grid->children->first;
        child; child = (Cyb_WidgetNode*)child->base.next)
    {
        Cyb_HandleUIEvent(child->widget, event);
    }
}