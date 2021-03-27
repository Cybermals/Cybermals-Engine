#ifndef CYBLABEL_H
#define CYBLABEL_H

/** @file
 * @brief CybUI - Label API
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
 
#include "CybGrid.h"
 
 
#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup CybUI
 * @brief Cybermals Engine - UI Subsystem
 * @{
 */

//Enums
//=================================================================================
/** @brief Text alignment flags.
 */
enum Cyb_TextAlignFlags
{
    CYB_TEXT_HALIGN_LEFT   = 0x00, /**< Left-aligned text. */
    CYB_TEXT_HALIGN_CENTER = 0x01, /**< Center-aligned text (horizontally). */
    CYB_TEXT_HALIGN_RIGHT  = 0x02, /**< Right-aligned text. */
    CYB_TEXT_VALIGN_TOP    = 0x00, /**< Top-aligned text. */
    CYB_TEXT_VALIGN_CENTER = 0x04, /**< Center-aligned text (vertically). */
    CYB_TEXT_VALIGN_BOTTOM = 0x08  /**< Bottom-aligned text. */
};


//Functions
//=================================================================================
/** @brief Render text using the given renderer, font, position, and color.
 *
 * @param renderer Pointer to the renderer.
 * @param font Pointer to the font.
 * @param pos Text position (affected by the flags).
 * @param color Text color.
 * @param text The text to render.
 * @param flags Text alignment flags.
 */
CYBAPI void Cyb_RenderText(SDL_Renderer *renderer, TTF_Font *font, 
    const SDL_Point *pos, SDL_Color color, const char *text, int flags);
    
/** @brief Label drawing procedure.
 *
 * @param label Pointer to the label widget.
 * @param renderer Pointer to the renderer.
 */
CYBAPI void Cyb_DrawLabelProc(Cyb_Grid *label, SDL_Renderer *renderer);

/** @brief Create a new label.
 *
 * @return Pointer to the label.
 */
CYBAPI Cyb_Grid *Cyb_CreateLabel(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif