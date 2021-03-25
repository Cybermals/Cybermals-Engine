/*
CybUI - Test Program
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>

#include <CybUI.h>


//Macros
//===========================================================================
#define APP_TITLE        "CybUI Test"
#define WINDOW_X         SDL_WINDOWPOS_CENTERED
#define WINDOW_Y         SDL_WINDOWPOS_CENTERED
#define WINDOW_WIDTH     800
#define WINDOW_HEIGHT    600

#ifdef __android__
    #define WINDOW_FLAGS SDL_WINDOW_FULLSCREEN
#else
    #define WINDOW_FLAGS 0
#endif

#define FPS              60


//Globals
//===========================================================================
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;


//Entry Point
//===========================================================================
int main(int argc, char **argv)
{
    //Init CybUI
    if(Cyb_InitUI())
    {
        return 1;
    }
    
    //Create a window and renderer
    SDL_Log("%s", "Creating a window...");
    
    if(SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_FLAGS,
        &window, &renderer) == -1)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "Failed to create a window.");
        return 1;
    }
    
    SDL_SetWindowTitle(window, APP_TITLE);
    SDL_SetWindowPosition(window, WINDOW_X, WINDOW_Y);
    
    //Init UI
    SDL_Log("%s", "Loading UI...");
    Cyb_Grid *root = Cyb_LoadUI(renderer, "data/UI/UI.xml");
    
    if(!root)
    {
        return 1;
    }
    
    //Test ID lookup
    puts("Testing ID lookup...");
    Cyb_Grid *child = Cyb_GetGridByID(root, "child");
    
    if(!child)
    {
        puts("ID lookup failed.");
    }
    
    child = Cyb_GetGridByID(root, "trash"); //this one should fail
    
    if(child)
    {
        puts("ID lookup failed.");
    }
    
    //Add some text to the text box
    Cyb_Grid *textBox = Cyb_GetGridByID(root, "textbox1");
    
    if(!textBox)
    {
        puts("Failed to set textbox contents.");
        return 1;
    }
    
    Cyb_LoadText(textBox, "data/UI/UI.xml");
    
    //Add some items to the list box
    Cyb_Grid *listBox = Cyb_GetGridByID(root, "listbox1");
    
    if(!listBox)
    {
        puts("Failed to set list box contents.");
        return 1;
    }
    
    Cyb_InsertItem(listBox, CYB_LIST_START, "cat", NULL);
    Cyb_InsertItem(listBox, CYB_LIST_END, "cheetah", NULL);
    Cyb_InsertItem(listBox, 1, "dog", NULL);
    Cyb_InsertItem(listBox, CYB_LIST_END, "rat", NULL);
    Cyb_InsertItem(listBox, CYB_LIST_END, "fox", NULL);
    Cyb_InsertItem(listBox, CYB_LIST_END, "horse", NULL);
    
    //Main Loop
    while(TRUE)
    {
        //Process events
        SDL_Event event;
        
        while(SDL_PollEvent(&event))
        {
            //Quit Event?
            if(event.type == SDL_QUIT)
            {
                return 0;
            }
            //UI Mouse Motion Event?
            else if(event.type == CYB_UI_MOUSEMOTION)
            {
                Cyb_Grid *grid = (Cyb_Grid*)event.user.data1;
                printf("Mouse moved over widget '%s' at pos (%i, %i).\n",
                    grid->id, 0, 0);
            }
            //UI Mouse Button Down Event
            else if(event.type == CYB_UI_MOUSEBUTTONDOWN)
            {
                Cyb_Grid *grid = (Cyb_Grid*)event.user.data1;
                printf("Mouse button %i down over widget '%s' at pos (%i, %i).\n",
                    event.user.code, grid->id, 0, 0);
                    
                //Save text?
                if(grid->id && strcmp(grid->id, "button1") == 0)
                {
                    #ifdef __ANDROID__
                    char buf[256];
                    SDL_snprintf(buf, sizeof(buf), "%s/%s",
                        SDL_AndroidGetExternalStoragePath(), "UI2.xml");
                    Cyb_SaveText(textBox, buf);
                    #else
                    Cyb_SaveText(textBox, "data/UI/UI2.xml");
                    #endif
                }
            }
            //UI Mouse Button Up Event
            else if(event.type == CYB_UI_MOUSEBUTTONUP)
            {
                Cyb_Grid *grid = (Cyb_Grid*)event.user.data1;
                printf("Mouse button %i up over widget '%s' at pos (%i, %i).\n",
                    event.user.code, grid->id, 0, 0);
            }
            
            Cyb_HandleUIEvent(root, &event);
        }
        
        //Clear window
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        
        //Update UI
        Cyb_DrawUI(root, renderer);
        
        //Swap buffers and limit framerate
        SDL_RenderPresent(renderer);
        Cyb_NextFrame(FPS);
    }
    
    return 0;
}