/*
CybRender - Texture API
*/


#include <SDL2/SDL_opengl.h>

#include "CybObjects.h"
#include "CybTexture.h"


//Structures
//=================================================================================
struct Cyb_Texture
{
    Cyb_Object base;
    int width;
    int height;
    int format;
    GLuint tex;
};


//Functions
//=================================================================================
void Cyb_FreeTexture(Cyb_Texture *tex)
{}


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