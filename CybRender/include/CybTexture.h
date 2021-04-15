#ifndef CYBTEXTURE_H
#define CYBTEXTURE_H

/** @file
 * @brief CybRender - Texture API
 */
 
#include "CybCommon.h"
#include "CybRenderer.h"
 
 
#ifdef __cplusplus
extern "C" {
#endif

//Enums
//=================================================================================
enum Cyb_PixelFormat
{
    CYB_PIXEL_FORMAT_UNKNOWN,
    CYB_PIXEL_FORMAT_RGB,
    CYB_PIXEL_FORMAT_RGBA,
    CYB_PIXEL_FORMAT_BGR,
    CYB_PIXEL_FORMAT_BGRA
};


enum Cyb_TextureFilter
{
    CYB_TEXTURE_FILTER_NEAREST,
    CYB_TEXTURE_FILTER_LINEAR
};


//Types
//=================================================================================
typedef struct Cyb_Texture Cyb_Texture;


//Structures
//=================================================================================
typedef struct
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Cyb_PixelRGB;


typedef struct
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} Cyb_PixelRGBA;


typedef struct
{
    unsigned char b;
    unsigned char g;
    unsigned char r;
} Cyb_PixelBGR;


typedef struct
{
    unsigned char b;
    unsigned char g;
    unsigned char r;
    unsigned char a;
} Cyb_PixelBGRA;


//Functions
//=================================================================================
CYBAPI Cyb_Texture *Cyb_CreateTexture(Cyb_Renderer *renderer);
CYBAPI void Cyb_UpdateTexture(Cyb_Renderer *renderer, Cyb_Texture *tex, int width,
    int height, int format, const void *pixels);
CYBAPI void Cyb_SetTextureFilters(Cyb_Renderer *renderer, Cyb_Texture *tex, 
    int min, int mag);
CYBAPI void Cyb_SelectTexture(Cyb_Renderer *renderer, Cyb_Texture *tex, 
    int texUnit);

#ifdef __cplusplus
}
#endif

#endif