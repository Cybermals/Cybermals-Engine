#ifndef CYBUILOADER_H
#define CYBUILOADER_H

/** @file
 * @brief CybUI - UI Loader API
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
 
#include "CybCommon.h"
#include "CybGrid.h"

 
#ifdef __cplusplus
extern "C" {
#endif

//Macros
//=================================================================================
/** @brief Load a UI texture.
 *
 * @param renderer Pointer to the renderer that will be used to draw the UI.
 * @param filename The name of the texture file to load.
 *
 * @return Pointer to the texture or NULL on failure.
 */
#define Cyb_LoadUITexture(renderer, filename) Cyb_LoadUITextureRW(renderer, \
    SDL_RWFromFile(filename, "r"), TRUE, filename)
    
/** @brief Load a UI font.
 *
 * @param filename The name of the font file to load.
 * @param size The size of the font to load.
 *
 * @return Pointer to the font or NULL on failure.
 */
#define Cyb_LoadUIFont(filename, size) Cyb_LoadUIFontRW( \
    SDL_RWFromFile(filename, "r"), TRUE, size, filename)
    
/** @brief Load a UI definition file and return the resulting widget tree.
 *
 * @param renderer Pointer to the renderer that will be used to draw the UI.
 * @param filename The name of the UI definition file.
 *
 * @return Pointer to the root widget.
 */
#define Cyb_LoadUI(renderer, filename) Cyb_LoadUIFromRW(renderer, \
    SDL_RWFromFile(filename, "r"), TRUE)


//Functions
//=================================================================================
/** @brief Initialize the UI loader.
 *
 * @return CYB_NO_ERROR on success or CYB_ERROR on failure.
 */
CYBAPI int Cyb_InitUILoader(void);

/** @brief Finalize the UI loader.
 */
CYBAPI void Cyb_FiniUILoader(void);

/** @brief Load a UI texture.
 *
 * @param renderer Pointer to the renderer that will be used to draw the UI.
 * @param file Pointer to the file to load.
 * @param doClose TRUE if the file should be closed after loading it.
 * @param id The ID to be used for this resource.
 *
 * @return Pointer to the texture or NULL on failure.
 */
CYBAPI SDL_Texture *Cyb_LoadUITextureRW(SDL_Renderer *renderer, SDL_RWops *file, 
    int doClose, const char *id);
    
/** @brief Load a UI font.
 *
 * @param file Pointer to the file to load.
 * @param size The size of the font to load.
 * @param id The ID to be used for this resource.
 *
 * @return Pointer to the font or NULL on failure.
 */
CYBAPI TTF_Font *Cyb_LoadUIFontRW(SDL_RWops *file, int doClose, int size, 
    const char *id);
    
/** @brief Load a UI definition file and return the resulting widget tree.
 *
 * @param renderer Pointer to the renderer that will be used to draw the UI.
 * @param file Pointer to the file to load.
 * @param doClose TRUE if the file should be closed after loading it.
 *
 * @return Pointer to the root widget.
 */
CYBAPI Cyb_Grid *Cyb_LoadUIFromRW(SDL_Renderer *renderer, SDL_RWops *file, 
    int doClose);

#ifdef __cplusplus
}
#endif

#endif