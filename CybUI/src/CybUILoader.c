/*
CybUI - UI Loader API
*/

#include <stdio.h>
#include <string.h>

#include <expat.h>

#include "CybObjects.h"
#include "CybUILoader.h"
#include "CybWidgetList.h"

#define PARSER_BUF_SIZE 1024


//Globals
//=================================================================================
static Cyb_Grid *root = NULL;


//Functions
//=================================================================================
void *Cyb_StartUIElement(Cyb_List *stack, const XML_Char *name, 
    const XML_Char **attribs)
{
    //Grid widget?
    if(strcmp(name, "grid") == 0)
    {
        //Create new grid
        Cyb_Grid *grid = Cyb_CreateGrid();
        
        if(!grid)
        {
            return NULL;
        }
        
        //Initialize the grid
        for(const char **attrib = attribs; *attrib; attrib += 2)
        {
            //Rows attrib?
            if(strcmp(attrib[0], "rows") == 0)
            {
                sscanf(attrib[1], "%i", &grid->rows);
            }
            //Cols attrib?
            else if(strcmp(attrib[0], "cols") == 0)
            {
                sscanf(attrib[1], "%i", &grid->cols);
            }
            //Row attrib?
            else if(strcmp(attrib[0], "row") == 0)
            {
                sscanf(attrib[1], "%i", &grid->row);
            }
            //Col attrib?
            else if(strcmp(attrib[0], "col") == 0)
            {
                sscanf(attrib[1], "%i", &grid->col);
            }
            //Rowspan attrib?
            else if(strcmp(attrib[0], "rowspan") == 0)
            {
                sscanf(attrib[1], "%i", &grid->rowspan);
            }
            //Colspan attrib?
            else if(strcmp(attrib[0], "colspan") == 0)
            {
                sscanf(attrib[1], "%i", &grid->colspan);
            }
            //ID attrib?
            else if(strcmp(attrib[0], "id") == 0)
            {
                Cyb_SetGridID(grid, attrib[1]);
            }
            //Unknown attrib?
            else
            {
                SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                    "[CybUI] Unknown attribute for grid: '%s'", *attrib);
            }
        }
        
        //Push it
        Cyb_WidgetNode *node = (Cyb_WidgetNode*)Cyb_InsertListElm(stack, 
            CYB_LIST_END);
            
        if(!node)
        {
            return NULL;
        }
        
        node->widget = (Cyb_Grid*)Cyb_NewObjectRef((Cyb_Object*)grid);
        
        //Add to parent
        if(stack->len > 1)
        {
            node = (Cyb_WidgetNode*)Cyb_GetListElm(stack, -2);
            
            if(!node)
            {
                return NULL;
            }
            
            Cyb_AddChildGrid(node->widget, grid);
        }
    }
    //Unknown tag?
    else
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
            "[CybUI] Encountered unknown tag '%s'.", name);
    }
    
    //We need to keep a reference to the root widget
    if(stack->len == 1)
    {
        root = ((Cyb_WidgetNode*)Cyb_GetListElm(stack, CYB_LIST_START))->widget;
    }

    return NULL;
}


void *Cyb_EndUIElement(Cyb_List *stack, const XML_Char *name)
{
    //Pop the stack if the tag is a known tag
    if(strcmp(name, "grid") == 0)
    {
        Cyb_RemoveListElm(stack, CYB_LIST_END);
    }

    return NULL;
}


Cyb_Grid *Cyb_LoadUIFromRW(SDL_RWops *file, int doClose)
{
    //Is the file valid?
    if(!file)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[CybUI] %s", SDL_GetError());
        return NULL;
    }
    
    //Create a stack to hold the widgets as we build the widget tree
    Cyb_List *stack = Cyb_CreateWidgetList();
        
    if(!stack)
    {
        return NULL;
    }
    
    //Create an XML parser
    XML_Parser parser = XML_ParserCreate("UTF-8");
    
    if(!parser)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "[CybUI] Failed to create XML parser.");
        Cyb_FreeObject((Cyb_Object**)&stack);
    
        //Close the file?
        if(doClose)
        {
            SDL_RWclose(file);
        }
        
        return NULL;
    }
    
    XML_SetElementHandler(parser, (XML_StartElementHandler)&Cyb_StartUIElement,
        (XML_EndElementHandler)&Cyb_EndUIElement);
    XML_SetUserData(parser, stack);
    
    //Parse the UI definition file
    while(TRUE)
    {
        //Get the parser buffer
        void *buf = XML_GetBuffer(parser, PARSER_BUF_SIZE);
        
        if(!buf)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "[CybUI] Failed to create XML parser.");
            XML_ParserFree(parser);
            Cyb_FreeObject((Cyb_Object**)&stack);
    
            //Close the file?
            if(doClose)
            {
                SDL_RWclose(file);
            }
        
            return NULL;
        }
        
        //Fill the buffer with data
        int len = SDL_RWread(file, buf, sizeof(char), PARSER_BUF_SIZE);
        
        if(len != PARSER_BUF_SIZE)
        {
            //Parse the last part of the data
            XML_ParseBuffer(parser, len, TRUE);
            break;
        }
        
        //Parse the next part of the data
        XML_ParseBuffer(parser, len, FALSE);
    }
    
    //Free the XML parser
    XML_ParserFree(parser);
    
    //Free the stack
    Cyb_FreeObject((Cyb_Object**)&stack);
    
    //Close the file?
    if(doClose)
    {
        SDL_RWclose(file);
    }

    return root;
}