/*
CybUI - Button API
*/

#include "CybButton.h"
#include "CybLabel.h"


//Structures
//=================================================================================
typedef struct
{
    Cyb_Object base;
    int mode;
    SDL_Texture *up;
    SDL_Texture *down;
} Cyb_ButtonData;


//Functions
//=================================================================================
void Cyb_DrawButtonProc(Cyb_Grid *button, SDL_Renderer *renderer)
{
    //Call base draw procedure
    Cyb_DrawLabelProc(button, renderer);
}


void Cyb_HandleButtonEventProc(Cyb_Grid *button, const SDL_Event *event)
{
    //Call base event handler procedure
    Cyb_HandleGridEventProc(button, event);
    
    //Handle button events
    Cyb_ButtonData *data = (Cyb_ButtonData*)button->data;
    SDL_Point mousePos;
    
    switch(event->type)
    {
        //Mouse Motion
    case SDL_MOUSEMOTION:
    {
        //Pop the button up if the mouse left the button and the mode is push
        mousePos.x = event->motion.x;
        mousePos.y = event->motion.y;
        
        if(data->mode == CYB_BUTTONMODE_PUSH && 
            !SDL_PointInRect(&mousePos, &button->viewport))
        {
            button->bg = data->up;
        }
    
        break;
    }
        
        //Mouse Button Down
    case SDL_MOUSEBUTTONDOWN:
    {
        //Was this button pressed?
        mousePos.x = event->button.x;
        mousePos.y = event->button.y;
        
        if(SDL_PointInRect(&mousePos, &button->viewport))
        {
            //Push mode?
            if(data->mode == CYB_BUTTONMODE_PUSH)
            {
                button->bg = data->down;
            }
        }
        
        break;
    }
        
        //Mouse Button Up
    case SDL_MOUSEBUTTONUP:
    {
        //Was this button released?
        mousePos.x = event->button.x;
        mousePos.y = event->button.y;
        
        if(SDL_PointInRect(&mousePos, &button->viewport))
        {
            //Push mode?
            if(data->mode == CYB_BUTTONMODE_PUSH)
            {
                button->bg = data->up;
            }
            //Toggle mode?
            else if(data->mode == CYB_BUTTONMODE_TOGGLE)
            {
                button->bg = (button->bg == data->up ? data->down : data->up);
            }
        }
    
        break;
    }
    }
}


Cyb_Grid *Cyb_CreateButton(void)
{
    //Allocate a new button
    Cyb_Grid *button = Cyb_CreateLabel();
    
    if(!button)
    {
        return NULL;
    }
    
    //Initialize the button
    button->base.type = CYB_BUTTON;
    button->draw = &Cyb_DrawButtonProc;
    button->handleEvent = &Cyb_HandleButtonEventProc;
    button->data = Cyb_CreateObject(sizeof(Cyb_ButtonData), NULL, CYB_BUTTON);
    
    if(!button->data)
    {
        Cyb_FreeObject((Cyb_Object**)&button);
        return NULL;
    }
    
    Cyb_SetButtonTextures(button, NULL, NULL);
    Cyb_SetButtonMode(button, CYB_BUTTONMODE_PUSH);
    return button;
}


void Cyb_SetButtonTextures(Cyb_Grid *button, SDL_Texture *up, 
    SDL_Texture *down)
{
    //Ensure that the widget is a button
    if(button->base.type != CYB_BUTTON)
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "[CybUI] The widget passed to 'Cyb_SetButtonTextures' was not a button widget!");
        return;
    }

    //Set the button textures
    Cyb_ButtonData *data = (Cyb_ButtonData*)button->data;
    data->up = up;
    data->down = down;
    
    //Update the current texture
    button->bg = data->up;
}


void Cyb_SetButtonMode(Cyb_Grid *button, int mode)
{
    //Ensure that the widget is a button
    if(button->base.type != CYB_BUTTON)
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "[CybUI] The widget passed to 'Cyb_SetButtonMode' was not a button widget!");
        return;
    }
    
    //Set the button mode
    Cyb_ButtonData *data = (Cyb_ButtonData*)button->data;
    data->mode = mode;
}


int Cyb_GetButtonState(Cyb_Grid *button)
{
    //Ensure that the widget is a button
    if(button->base.type != CYB_BUTTON)
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "[CybUI] The widget passed to 'Cyb_GetButtonState' was not a button widget!");
        return FALSE;
    }
    
    //Return the button state
    Cyb_ButtonData *data = (Cyb_ButtonData*)button->data;
    return button->bg == data->down;
}