/*
CybUI - List Box API
*/

#include "CybLabel.h"
#include "CybListBox.h"


//Structures
//=================================================================================
typedef struct
{
    Cyb_Object base;
    int mode;
    SDL_Point scrollPos;
    int isScrolling;
    Cyb_List *items;
} Cyb_ListBoxData;


//Functions
//=================================================================================
void Cyb_FreeListBoxItem(Cyb_ListBoxItem *item)
{
    //Free the name and object
    if(item->name)
    {
        SDL_free(item->name);
    }
    
    if(item->data)
    {
        Cyb_FreeObject((Cyb_Object**)&item->data);
    }
}


void Cyb_FreeListBoxData(Cyb_ListBoxData *obj)
{
    //Free the items list
    Cyb_FreeObject((Cyb_Object**)&obj->items);
}


void Cyb_DrawListBoxProc(Cyb_Grid *listBox, SDL_Renderer *renderer)
{
    //Call the default draw procedure
    Cyb_DrawGridProc((Cyb_Grid*)listBox, renderer);
    
    //Draw each list box item starting with the first visible item
    Cyb_ListBoxData *data = (Cyb_ListBoxData*)listBox->data;
    SDL_Point pos;
    pos.x = -data->scrollPos.x;
    pos.y = -data->scrollPos.y;
    int line = 0;
    int lineInc = TTF_FontLineSkip(listBox->font);
    
    for(Cyb_ListBoxItem *item = (Cyb_ListBoxItem*)data->items->first;
        item; item = (Cyb_ListBoxItem*)item->base.next)
    {
        //Skip this line if it is out of bounds
        if(pos.y + lineInc < 0 || pos.y > listBox->viewport.h)
        {
            line++;
            pos.y += lineInc;
            continue;
        }
        
        //Render the background of the list item if it is selected
        if(item->isSelected)
        {
            SDL_SetRenderDrawColor(renderer, listBox->fg.r, listBox->fg.g, 
                listBox->fg.b, listBox->fg.a);
            SDL_Rect rect;
            rect.x = 0;
            rect.y = pos.y;
            rect.w = listBox->viewport.w;
            rect.h = lineInc;
            SDL_RenderDrawRect(renderer, &rect);
        }
        
        //Render the name of the list item
        Cyb_RenderText(renderer, listBox->font, &pos, listBox->fg, item->name,
            CYB_TEXT_HALIGN_LEFT | CYB_TEXT_VALIGN_TOP);
        line++;
        pos.y += lineInc;
    }
}


void Cyb_HandleListBoxEventProc(Cyb_Grid *listBox, const SDL_Event *event)
{
    //Call the default event handler
    Cyb_HandleGridEventProc((Cyb_Grid*)listBox, event);
    
    //Handle the event
    Cyb_ListBoxData *data = (Cyb_ListBoxData*)listBox->data;
    
    switch(event->type)
    {
        //Mouse Motion
    case SDL_MOUSEMOTION:
    {
        //Update scrolling
        if(data->isScrolling)
        {
            data->scrollPos.x -= event->motion.xrel;
            data->scrollPos.y -= event->motion.yrel;
            
            if(data->scrollPos.x < 0)
            {
                data->scrollPos.x = 0;
            }
            
            if(data->scrollPos.y < 0)
            {
                data->scrollPos.y = 0;
            }
        }
        
        break;
    }
    
        //Mouse Button Down
    case SDL_MOUSEBUTTONDOWN:
    {
        //Ignore this if the mouse was outside the widget
        SDL_Point mousePos;
        mousePos.x = event->button.x;
        mousePos.y = event->button.y;
        
        if(!SDL_PointInRect(&mousePos, &listBox->viewport))
        {
            return;
        }
        
        //Turn on scrolling
        data->isScrolling = TRUE;
        
        //Update current selection
        if(!(data->mode & CYB_LISTBOX_MULTISELECT))
        {
            //Clear all previous selections
            for(Cyb_ListBoxItem *item = (Cyb_ListBoxItem*)data->items->first;
                item; item = (Cyb_ListBoxItem*)item->base.next)
            {
                item->isSelected = FALSE;
            }
        }
    
        SDL_Point localPos;
        Cyb_GlobalToLocal(listBox, &mousePos, &localPos);
        int i = (localPos.y + data->scrollPos.y) / TTF_FontLineSkip(listBox->font);
        Cyb_ListBoxItem *item = (Cyb_ListBoxItem*)Cyb_GetListElm(data->items, i);
        
        if(item)
        {
            item->isSelected = !item->isSelected;
        }
    
        break;
    }
    
        //Mouse Button Up
    case SDL_MOUSEBUTTONUP:
    {
        //Turn off scrolling
        data->isScrolling = FALSE;
        break;
    }
    }
}


Cyb_Grid *Cyb_CreateListBox(void)
{
    //Allocate new list box
    Cyb_Grid *listBox = Cyb_CreateGrid();
    
    if(!listBox)
    {
        return NULL;
    }
    
    //Initialize list box
    listBox->base.type = CYB_LISTBOX;
    listBox->draw = &Cyb_DrawListBoxProc;
    listBox->handleEvent = &Cyb_HandleListBoxEventProc;
    listBox->data = Cyb_CreateObject(sizeof(Cyb_ListBoxData),
        (Cyb_FreeProc)&Cyb_FreeListBoxData, CYB_LISTBOX);
        
    if(!listBox->data)
    {
        Cyb_FreeObject((Cyb_Object**)&listBox);
        return NULL;
    }
    
    Cyb_ListBoxData *data = (Cyb_ListBoxData*)listBox->data;
    data->mode = 0;
    data->scrollPos.x = 0;
    data->scrollPos.y = 0;
    data->isScrolling = FALSE;
    data->items = Cyb_CreateList(sizeof(Cyb_ListBoxItem),
        (Cyb_FreeNodeProc)&Cyb_FreeListBoxItem);
        
    if(!data->items)
    {
        Cyb_FreeObject((Cyb_Object**)&listBox);
        return NULL;
    }

    return listBox;
}


void Cyb_SetListBoxMode(Cyb_Grid *listBox, int mode)
{
    //Ensure that the widget is a list box
    if(listBox->base.type != CYB_LISTBOX)
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "[CybUI] The widget passed to 'Cyb_SetListBoxMode' was not a list box!");
        return;
    }
    
    //Set the new list box mode
    Cyb_ListBoxData *data = (Cyb_ListBoxData*)listBox->data;
    data->mode = mode;
}


void Cyb_ToggleListBoxMode(Cyb_Grid *listBox, int mode)
{
    //Ensure that the widget is a list box
    if(listBox->base.type != CYB_LISTBOX)
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "[CybUI] The widget passed to 'Cyb_ToggleListBoxMode' was not a list box!");
        return;
    }
    
    //Toggle the given list box modes
    Cyb_ListBoxData *data = (Cyb_ListBoxData*)listBox->data;
    data->mode ^= mode;
}


void Cyb_SetListBoxScrollPos(Cyb_Grid *listBox, int x, int y)
{
    //Ensure that the widget is a list box
    if(listBox->base.type != CYB_LISTBOX)
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "[CybUI] The widget passed to 'Cyb_ToggleListBoxMode' was not a list box!");
        return;
    }
    
    //Set the new scroll pos
    Cyb_ListBoxData *data = (Cyb_ListBoxData*)listBox->data;
    data->scrollPos.x = x;
    data->scrollPos.y = y;
}


void Cyb_InsertItem(Cyb_Grid *listBox, int i, const char *name, 
    Cyb_Object *itemData)
{
    //Ensure that the widget is a list box
    if(listBox->base.type != CYB_LISTBOX)
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "[CybUI] The widget passed to 'Cyb_InsertItem' was not a list box!");
        return;
    }
    
    //Insert a new list box item
    Cyb_ListBoxData *data = (Cyb_ListBoxData*)listBox->data;
    Cyb_ListBoxItem *item = (Cyb_ListBoxItem*)Cyb_InsertListElm(data->items, i);
    
    if(!item)
    {
        return;
    }
    
    item->isSelected = FALSE;
    item->name = (char*)SDL_malloc(strlen(name) + 1);
    
    if(!item->name)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", "[CybUI] Out of Memory");
        Cyb_RemoveListElm(data->items, i);
        return;
    }
    
    strcpy(item->name, name);
    item->data = (itemData ? Cyb_NewObjectRef(itemData) : NULL);
}


void Cyb_RemoveItem(Cyb_Grid *listBox, int i)
{
    //Ensure that the widget is a list box
    if(listBox->base.type != CYB_LISTBOX)
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "[CybUI] The widget passed to 'Cyb_RemoveItem' was not a list box!");
        return;
    }
    
    //Remove the given list box item
    Cyb_ListBoxData *data = (Cyb_ListBoxData*)listBox->data;
    Cyb_RemoveListElm(data->items, i);
}


Cyb_ListBoxItem *Cyb_GetItem(Cyb_Grid *listBox, int i)
{
    //Ensure that the widget is a list box
    if(listBox->base.type != CYB_LISTBOX)
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "[CybUI] The widget passed to 'Cyb_GetItem' was not a list box!");
        return NULL;
    }
    
    //Get the requested list box item
    Cyb_ListBoxData *data = (Cyb_ListBoxData*)listBox->data;
    return (Cyb_ListBoxItem*)Cyb_GetListElm(data->items, i);
}