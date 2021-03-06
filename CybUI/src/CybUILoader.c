/*
CybUI - UI Loader API
*/

#include <string.h>

#include <expat.h>

#include "CybObjects.h"
#include "CybUILoader.h"
#include "CybWidgetList.h"

#define PARSER_BUF_SIZE 1024


//Functions
//=================================================================================
void *Cyb_StartUIElement(Cyb_List *stack, const XML_Char *name, 
    const XML_Char **attribs)
{
    //Grid widget?
    if(strcmp(name, "grid") == 0)
    {
        SDL_Log("%s", "Found grid tag.");
    }
    //Unknown tag?
    else
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
            "[CybUI] Encountered unknown tag '%s'.", name);
    }
    
    //The root widget needs to be pushed again to make sure it will persist
    if(stack->len == 1)
    {
        //<======= push the root widget a second time here
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

    return NULL;
}