/*
CybUI - UI Loader API
*/

#include <stdio.h>
#include <string.h>

#include <SDL2/SDL_image.h>
#include <expat.h>

#include "CybButton.h"
#include "CybLabel.h"
#include "CybObjects.h"
#include "CybTextBox.h"
#include "CybUILoader.h"
#include "CybWidgetList.h"

#define PARSER_BUF_SIZE 1024


//Structures
//=================================================================================
typedef struct
{
    Cyb_ListNode base;
    char *id;
    SDL_Renderer *renderer;
    SDL_Texture *tex;
} Cyb_TexCacheNode;


typedef struct
{
    Cyb_ListNode base;
    char *id;
    int size;
    TTF_Font *font;
} Cyb_FontCacheNode;


//Globals
//=================================================================================
static int refCnt = 0;

static SDL_Renderer *cachedRenderer = NULL;
static Cyb_Grid *root = NULL;
static Cyb_List *texCache = NULL;
static Cyb_List *fontCache = NULL;

static char tmpBuf[1024];
static char tmpBuf2[1024];


//Functions
//=================================================================================
void Cyb_FreeTexCacheNode(Cyb_TexCacheNode *node)
{
    //Free the ID string
    if(node->id)
    {
        SDL_free(node->id);
    }
}


void Cyb_FreeFontCacheNode(Cyb_FontCacheNode *node)
{
    //Free the ID string
    if(node->id)
    {
        SDL_free(node->id);
    }
    
    //Close the font
    if(node->font)
    {
        TTF_CloseFont(node->font);
    }
}


void *Cyb_StartUIElement(Cyb_List *stack, const XML_Char *name, 
    const XML_Char **attribs)
{
    Cyb_Grid *grid;
    
    //Grid widget?
    if(strcmp(name, "grid") == 0)
    {
        //Create new grid
        grid = Cyb_CreateGrid();
        
        if(!grid)
        {
            return NULL;
        }
    }
    //Label widget?
    else if(strcmp(name, "label") == 0)
    {
        //Create new label
        grid = Cyb_CreateLabel();
        
        if(!grid)
        {
            return NULL;
        }
    }
    //Button widget?
    else if(strcmp(name, "button") == 0)
    {
        //Create new button
        grid = Cyb_CreateButton();
        
        if(!grid)
        {
            return NULL;
        }
    }
    //TextBox widget?
    else if(strcmp(name, "textbox") == 0)
    {
        //Create new text box
        grid = Cyb_CreateTextBox();
        
        if(!grid)
        {
            return NULL;
        }
    }
    //Unknown tag?
    else
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
            "[CybUI] Encountered unknown tag '%s'.", name);
        return NULL;
    }
    
    //Initialize the grid (these attribs are available for all widget types)
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
        //BG attrib?
        else if(strcmp(attrib[0], "bg") == 0)
        {
            grid->bg = Cyb_LoadUITexture(cachedRenderer, attrib[1]);
        }
        //FG attrib?
        else if(strcmp(attrib[0], "fg") == 0)
        {
            int r = 0;
            int g = 0;
            int b = 0;
            int a = 255;
            sscanf(attrib[1], "%i %i %i %i", &r, &g, &b, &a);
            
            grid->fg.r = r;
            grid->fg.g = g;
            grid->fg.b = b;
            grid->fg.a = a;
        }
        //Visible attrib?
        else if(strcmp(attrib[0], "visible") == 0)
        {
            grid->visible = (strcmp(attrib[1], "true") == 0);
        }
        //Font attrib?
        else if(strcmp(attrib[0], "font") == 0)
        {
            int size = 18;
            sscanf(attrib[1], "%s %i", tmpBuf, &size);
            grid->font = Cyb_LoadUIFont(tmpBuf, size);
        }
        //Text attrib?
        else if(strcmp(attrib[0], "text") == 0)
        {
            Cyb_SetGridText(grid, attrib[1]);
        }
        //Button textures attrib?
        else if(strcmp(attrib[0], "btntex") == 0)
        {
            sscanf(attrib[1], "%s %s", tmpBuf, tmpBuf2);
            Cyb_SetButtonTextures(grid, Cyb_LoadUITexture(cachedRenderer, tmpBuf),
                Cyb_LoadUITexture(cachedRenderer, tmpBuf2));
        }
        //Button mode attrib?
        else if(strcmp(attrib[0], "btnmode") == 0)
        {
            //Push mode?
            if(strcmp(attrib[1], "push") == 0)
            {
                Cyb_SetButtonMode(grid, CYB_BUTTONMODE_PUSH);
            }
            //Toggle mode?
            else if(strcmp(attrib[1], "toggle") == 0)
            {
                Cyb_SetButtonMode(grid, CYB_BUTTONMODE_TOGGLE);
            }
        }
        //TextBox multiline attrib?
        else if(strcmp(attrib[0], "multiline") == 0)
        {   
            //Multiline?
            if(strcmp(attrib[1], "true") == 0)
            {
                Cyb_ToggleTextBoxMode(grid, CYB_TEXTBOX_MULTILINE);
            }
        }
        //TextBox readonly attrib?
        else if(strcmp(attrib[0], "readonly") == 0)
        {
            //Read-only?
            if(strcmp(attrib[1], "true") == 0)
            {
                Cyb_ToggleTextBoxMode(grid, CYB_TEXTBOX_READONLY);
            }
        }
        //Unknown attrib?
        else
        {
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                "[CybUI] Unknown attribute for grid: '%s'", *attrib);
        }
    }
    
    //Push the grid
    Cyb_WidgetNode *node = (Cyb_WidgetNode*)Cyb_InsertListElm(stack, 
        CYB_LIST_END);
            
    if(!node)
    {
        return NULL;
    }
        
    node->widget = (Cyb_Grid*)Cyb_NewObjectRef((Cyb_Object*)grid);
        
    //Add to parent grid
    if(stack->len > 1)
    {
        node = (Cyb_WidgetNode*)Cyb_GetListElm(stack, -2);
            
        if(!node)
        {
            return NULL;
        }
            
        Cyb_AddChildGrid(node->widget, grid);
    }
    
    //We need to keep a reference to the root widget
    if(stack->len == 1)
    {
        root = ((Cyb_WidgetNode*)Cyb_GetListElm(stack, CYB_LIST_START))->widget;
        root = (Cyb_Grid*)Cyb_NewObjectRef((Cyb_Object*)root);
    }
    
    //Free the widget we created
    Cyb_FreeObject((Cyb_Object**)&grid);
    return NULL;
}


void *Cyb_EndUIElement(Cyb_List *stack, const XML_Char *name)
{
    //Pop the stack if the tag is a known tag
    if(strcmp(name, "grid") == 0 || strcmp(name, "label") == 0 ||
        strcmp(name, "button") == 0 || strcmp(name, "textbox") == 0)
    {
        Cyb_RemoveListElm(stack, CYB_LIST_END);
    }

    return NULL;
}


int Cyb_InitUILoader(void)
{
    //Update ref count
    if(refCnt++ == 0)
    {
        //Initialize texture cache
        texCache = Cyb_CreateList(sizeof(Cyb_TexCacheNode), 
            (CybFreeNodeProc)&Cyb_FreeTexCacheNode);
            
        if(!texCache)
        {
            refCnt = 0;
            return CYB_ERROR;
        }
        
        //Initialize font cache
        fontCache = Cyb_CreateList(sizeof(Cyb_FontCacheNode),
            (CybFreeNodeProc)Cyb_FreeFontCacheNode);
            
        if(!fontCache)
        {
            refCnt = 0;
            return CYB_ERROR;
        }
    }
    
    return CYB_NO_ERROR;
}


void Cyb_FiniUILoader(void)
{
    //Update ref count
    if(--refCnt == 0)
    {
        //Free texture and font cache
        Cyb_FreeObject((Cyb_Object**)&texCache);
        Cyb_FreeObject((Cyb_Object**)&fontCache);
    }
}


SDL_Texture *Cyb_LoadUITextureRW(SDL_Renderer *renderer, SDL_RWops *file, 
    int doClose, const char *id)
{
    //Is this texture in the texture cache?
    for(Cyb_TexCacheNode *node = (Cyb_TexCacheNode*)texCache->first;
        node; node = (Cyb_TexCacheNode*)node->base.next)
    {
        //Is this the requested texture?
        if(node->renderer == renderer && strcmp(node->id, id) == 0)
        {
            return node->tex;
        }
    }
    
    //If the ID is "none", create a special 1x1 transparent texture
    SDL_Texture *tex;
    
    if(strcmp(id, "none") == 0)
    {
        tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
            SDL_TEXTUREACCESS_STATIC, 1, 1);
            
        if(!tex)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[CybUI] %s",
                SDL_GetError());
            return NULL;
        }
        
        SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    }
    else
    {
        //Load image
        SDL_Surface *image = IMG_Load_RW(file, doClose);
    
        if(!image)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                "[CybUI] Failed to load texture '%s'", id);
            return NULL;
        }
    
        //Create texture from image
        tex = SDL_CreateTextureFromSurface(renderer, image);
    
        if(!tex)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[CybUI] %s", 
                SDL_GetError());
            return NULL;
        }
        
        SDL_FreeSurface(image);
    }
    
    //Cache the texture and return it
    Cyb_TexCacheNode *node = (Cyb_TexCacheNode*)Cyb_InsertListElm(texCache,
        CYB_LIST_END);
        
    if(!node)
    {
        return NULL;
    }
    
    node->id = (char*)SDL_malloc(strlen(id) + 1);
    
    if(!node->id)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", "[CybUI] Out of Memory");
        return NULL;
    }
    
    strcpy(node->id, id);
    node->renderer = renderer;
    node->tex = tex;
    SDL_Log("[CybUI] Texture '%s' loaded and cached.", id);
    return tex;
}


TTF_Font *Cyb_LoadUIFontRW(SDL_RWops *file, int doClose, int size, 
    const char *id)
{
    //Is this font already in the font cache?
    for(Cyb_FontCacheNode *node = (Cyb_FontCacheNode*)fontCache->first;
        node; node = (Cyb_FontCacheNode*)node->base.next)
    {
        //Is this the requested font?
        if(node->size == size && strcmp(node->id, id) == 0)
        {
            return node->font;
        }
    }
    
    //Load the font
    TTF_Font *font = TTF_OpenFontRW(file, doClose, size);
    
    if(!font)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
            "[CybUI] Failed to load font '%s'.", id);
        return NULL;
    }
    
    //Cache the font and return it
    Cyb_FontCacheNode *node = (Cyb_FontCacheNode*)Cyb_InsertListElm(fontCache,
        CYB_LIST_END);
        
    if(!node)
    {
        return NULL;
    }
    
    node->id = (char*)SDL_malloc(strlen(id) + 1);
    
    if(!node->id)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", "[CybUI] Out of Memory");
        return NULL;
    }
    
    strcpy(node->id, id);
    node->size = size;
    node->font = font;
    SDL_Log("[CybUI] Font '%s' size %ipt loaded and cached.", id, size);
    return font;
}


Cyb_Grid *Cyb_LoadUIFromRW(SDL_Renderer *renderer, SDL_RWops *file, int doClose)
{
    //Is the file valid?
    if(!file)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[CybUI] %s", SDL_GetError());
        return NULL;
    }
    
    //Keep a global reference to the renderer we will be using
    cachedRenderer = renderer;
    
    //Create a stack to hold the widgets as we build the widget tree
    Cyb_List *stack = Cyb_CreateWidgetList();
        
    if(!stack)
    {
        //Close the file?
        if(doClose)
        {
            SDL_RWclose(file);
        }
        
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