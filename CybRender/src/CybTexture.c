/*
CybRender - Texture API
*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>

#include "CybObjects.h"
#include "CybTexture.h"


//Structures
//=================================================================================
struct Cyb_Texture
{
    Cyb_Object base;
    Cyb_Renderer *renderer;
    int width;
    int height;
    int format;
    GLuint tex;
};


typedef struct
{
    Cyb_ListNode base;
    char *id;
    Cyb_Texture *tex;
} Cyb_TextureCacheNode;


//Globals
//=================================================================================
static Cyb_List *textureCache = NULL;


//Functions
//=================================================================================
static void Cyb_FreeTextureCache(void)
{
    //Free texture cache
    if(textureCache)
    {
        Cyb_FreeObject((Cyb_Object**)&textureCache);
    }
    
    //Quit SDL2_image
    IMG_Quit();
}


static void Cyb_FreeTexture(Cyb_Texture *tex)
{
    //Free the texture
    if(tex->tex)
    {
        glDeleteTextures(1, &tex->tex);
    }
}


static void Cyb_FreeTextureCacheNode(Cyb_TextureCacheNode *node)
{
    //Free the ID
    if(node->id)
    {
        SDL_free(node->id);
    }
    
    //Free the texture
    if(node->tex)
    {
        Cyb_FreeObject((Cyb_Object**)&node->tex);
    }
}


Cyb_Texture *Cyb_CreateTexture(Cyb_Renderer *renderer)
{
    //Allocate new texture
    Cyb_Texture *tex = (Cyb_Texture*)Cyb_CreateObject(sizeof(Cyb_Texture),
        (Cyb_FreeProc)&Cyb_FreeTexture, CYB_TEXTURE);
        
    if(!tex)
    {
        return NULL;
    }
    
    //Initialize the texture
    tex->renderer = renderer;
    tex->width = 0;
    tex->height = 0;
    tex->format = CYB_PIXEL_FORMAT_UNKNOWN;
    Cyb_SelectRenderer(renderer);
    glGenTextures(1, &tex->tex);
    
    if(!tex->tex)
    {
        Cyb_FreeObject((Cyb_Object**)&tex);
        return NULL;
    }
    
    return tex;
}


Cyb_Texture *Cyb_LoadTextureRW(Cyb_Renderer *renderer, SDL_RWops *file, 
    int doClose, const char *id)
{
    //Ignore this if the file is NULL.
    if(!file)
    {
        return NULL;
    }
    
    //Initialize SDL2_image and the texture cache upon first use
    if(!textureCache)
    {
        //Initialize SDL2_image
        int flags = IMG_INIT_PNG | IMG_INIT_JPG;
        
        if(IMG_Init(flags) != flags)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s",
                "[CybRender] Failed to initialize SDL2_image.");
            return NULL;
        }
        
        //Initialize the texture cache
        textureCache = Cyb_CreateList(sizeof(Cyb_TextureCacheNode), 
            (Cyb_FreeNodeProc)&Cyb_FreeTextureCacheNode);
            
        if(!textureCache)
        {
            if(doClose)
            {
                SDL_RWclose(file);
            }
        
            return NULL;
        }
        
        atexit(&Cyb_FreeTextureCache);
    }
    
    //Return cached texture if it has already been loaded
    for(Cyb_TextureCacheNode *node = (Cyb_TextureCacheNode*)textureCache->first;
        node; node = (Cyb_TextureCacheNode*)node->base.next)
    {
        //Is this the requested texture?
        if(strcmp(node->id, id) == 0)
        {
            return (Cyb_Texture*)Cyb_NewObjectRef((Cyb_Object*)node->tex);
        }
    }
    
    //Load the texture image
    SDL_Surface *img = IMG_Load_RW(file, doClose);
    
    if(!img)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
            "[CybRender] Failed to load texture '%s'.", id);
        return NULL;
    }
    
    //Create the texture
    Cyb_Texture *tex = Cyb_CreateTexture(renderer);
    
    if(!tex)
    {
        SDL_FreeSurface(img);
        return NULL;
    }
    
    //Update the texture
    int format = CYB_PIXEL_FORMAT_UNKNOWN;
    
    switch(img->format->format)
    {
        //RGB
    case SDL_PIXELFORMAT_RGB24:
        format = CYB_PIXEL_FORMAT_RGB;
        break;
        
        //RGBA
    case SDL_PIXELFORMAT_RGBA32:
        format = CYB_PIXEL_FORMAT_RGBA;
        break;
        
        //BGR
    case SDL_PIXELFORMAT_BGR24:
        format = CYB_PIXEL_FORMAT_BGR;
        break;
        
        //ABGR
    case SDL_PIXELFORMAT_BGRA32:
        format = CYB_PIXEL_FORMAT_BGRA;
        break;
    }
    
    Cyb_UpdateTexture(renderer, tex, img->w, img->h, format, img->pixels);
    
    //Free the image and cache the texture
    SDL_FreeSurface(img);
    Cyb_TextureCacheNode *node = (Cyb_TextureCacheNode*)Cyb_InsertListElm(
        textureCache, CYB_LIST_END);
        
    if(!node)
    {
        Cyb_FreeObject((Cyb_Object**)&tex);
        return NULL;
    }
    
    node->id = SDL_malloc(strlen(id) + 1);
    
    if(!node->id)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s",
            "[CybRender] Out of Memory");
        Cyb_FreeObject((Cyb_Object**)&tex);
        return NULL;
    }
    
    strcpy(node->id, id);
    node->tex = (Cyb_Texture*)Cyb_NewObjectRef((Cyb_Object*)tex);
    SDL_Log("[CybRender] Texture '%s' loaded and cached.", id);
    return tex;
}


void Cyb_UpdateTexture(Cyb_Renderer *renderer, Cyb_Texture *tex, int width,
    int height, int format, const void *pixels)
{
    //Select renderer
    Cyb_SelectRenderer(renderer);
    
    //Update the texture image
    glBindTexture(GL_TEXTURE_2D, tex->tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    GLenum glFormat = 0;
    
    switch(format)
    {
        //RGB
    case CYB_PIXEL_FORMAT_RGB:
        glFormat = GL_RGB;
        break;
        
        //RGBA
    case CYB_PIXEL_FORMAT_RGBA:
        glFormat = GL_RGBA;
        break;
        
        //BGR
    case CYB_PIXEL_FORMAT_BGR:
        glFormat = GL_BGR;
        break;
        
        //ABGR
    case CYB_PIXEL_FORMAT_BGRA:
        glFormat = GL_BGRA;
        break;
        
        //Unknown
    default:
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "[CybRender] Unknown pixel format.");
        break;
    }
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, glFormat, 
        GL_UNSIGNED_BYTE, pixels);
}


void Cyb_SetTextureFilters(Cyb_Renderer *renderer, Cyb_Texture *tex, 
    int min, int mag)
{
    //Select renderer
    Cyb_SelectRenderer(renderer);
    
    //Set texture filters
    GLenum minFilter = 0;
    GLenum magFilter = 0;
    
    switch(min)
    {
        //Nearest
    case CYB_TEXTURE_FILTER_NEAREST:
        minFilter = GL_NEAREST;
        break;
        
        //Linear
    case CYB_TEXTURE_FILTER_LINEAR:
        minFilter = GL_LINEAR;
        break;
    }
    
    switch(mag)
    {
        //Nearest
    case CYB_TEXTURE_FILTER_NEAREST:
        magFilter = GL_NEAREST;
        break;
        
        //Linear
    case CYB_TEXTURE_FILTER_LINEAR:
        magFilter = GL_LINEAR;
        break;
    }
    
    glBindTexture(GL_TEXTURE_2D, tex->tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
}


void Cyb_SelectTexture(Cyb_Renderer *renderer, Cyb_Texture *tex, 
    int texUnit)
{
    //Select renderer
    Cyb_SelectRenderer(renderer);
    
    //Set the texture unit and bind the texture
    Cyb_GLExtAPI *glExtAPI = Cyb_GetGLExtAPI(renderer);
    glExtAPI->ActiveTexture(GL_TEXTURE0 + texUnit);
    glBindTexture(GL_TEXTURE_2D, tex->tex);
}