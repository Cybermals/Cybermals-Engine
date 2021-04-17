#ifndef CYBTEXTURE_H
#define CYBTEXTURE_H

/** @file
 * @brief CybRender - Texture API
 */
 
#include "CybCommon.h"
#include "CybRenderer.h"

#define Cyb_LoadTexture(renderer, filename) Cyb_LoadTextureRW(renderer, \
    SDL_RWFromFile(filename, "r"), TRUE, filename)
 
 
#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup CybRender
 * @brief Cybermals Engine - Renderer Subsystem
 * @{
 */

//Enums
//=================================================================================
/** @brief Pixel formats.
 */
enum Cyb_PixelFormat
{
    CYB_PIXEL_FORMAT_UNKNOWN,
    CYB_PIXEL_FORMAT_RGB,
    CYB_PIXEL_FORMAT_RGBA,
    CYB_PIXEL_FORMAT_BGR,
    CYB_PIXEL_FORMAT_BGRA
};


/** @brief Texture filters.
 */
enum Cyb_TextureFilter
{
    CYB_TEXTURE_FILTER_NEAREST,
    CYB_TEXTURE_FILTER_LINEAR
};


//Types
//=================================================================================
/** @brief Texture type.
 */
typedef struct Cyb_Texture Cyb_Texture;


//Structures
//=================================================================================
/** @brief RGB pixel.
 */
typedef struct
{
    unsigned char r; /**< Red. */
    unsigned char g; /**< Green. */
    unsigned char b; /**< Blue. */
} Cyb_PixelRGB;


/** @brief RGBA pixel.
 */
typedef struct
{
    unsigned char r; /**< Red. */
    unsigned char g; /**< Green. */
    unsigned char b; /**< Blue. */
    unsigned char a; /**< Alpha. */
} Cyb_PixelRGBA;


/** @brief BGR pixel.
 */
typedef struct
{
    unsigned char b; /**< Blue. */
    unsigned char g; /**< Green. */
    unsigned char r; /**< Red. */
} Cyb_PixelBGR;


/** @brief BGRA pixel.
 */
typedef struct
{
    unsigned char b; /**< Blue. */
    unsigned char g; /**< Green. */
    unsigned char r; /**< Red. */
    unsigned char a; /**< Alpha. */
} Cyb_PixelBGRA;


//Functions
//=================================================================================
/** @brief Create a new texture.
 *
 * @param renderer Pointer to the renderer.
 *
 * @return Pointer to the texture.
 */
CYBAPI Cyb_Texture *Cyb_CreateTexture(Cyb_Renderer *renderer);

/** @brief Load a texture from an open file.
 *
 * @param renderer Pointer to the renderer.
 * @param file Pointer to the file.
 * @param doClose TRUE to close the file after loading it.
 * @param id The ID of the new texture.
 */
CYBAPI Cyb_Texture *Cyb_LoadTextureRW(Cyb_Renderer *renderer, SDL_RWops *file, 
    int doClose, const char *id);
    
/** @brief Update a texture.
 *
 * @param renderer Pointer to the renderer.
 * @param tex Pointer to the texture.
 * @param width The new width of the texture.
 * @param height The new height of the texture.
 * @param format The new pixel format of the texture.
 * @param pixels Pointer to the new pixels.
 */
CYBAPI void Cyb_UpdateTexture(Cyb_Renderer *renderer, Cyb_Texture *tex, int width,
    int height, int format, const void *pixels);
    
/** @brief Set the texture filters of a texture.
 *
 * @param renderer Pointer to the renderer.
 * @param tex Pointer to the texture.
 * @param min The new minimize filter.
 * @param mag The new magnify filter.
 */
CYBAPI void Cyb_SetTextureFilters(Cyb_Renderer *renderer, Cyb_Texture *tex, 
    int min, int mag);
    
/** @brief Select a texture to be used for rendering a mesh.
 *
 * @param renderer Pointer to the renderer.
 * @param tex Pointer to the texture.
 * @param texUnit The texture unit to associate the texture with.
 */
CYBAPI void Cyb_SelectTexture(Cyb_Renderer *renderer, Cyb_Texture *tex, 
    int texUnit);
    
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif