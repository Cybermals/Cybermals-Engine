#ifndef CYBPROGRESSBAR_H
#define CYBPROGRESSBAR_H

/** @file
 * @brief CybUI - Progress Bar API
 */
 
#include "CybGrid.h"

 
#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup CybUI
 * @brief Cybermals Engine - UI Subsystem
 * @{
 */

//Functions
//==================================================================================
/** @brief Create a new progress bar.
 *
 * @return Pointer to the progress bar.
 */
CYBAPI Cyb_Grid *Cyb_CreateProgressBar(void);

/** @brief Set the foreground and background textures for a progress bar.
 *
 * @param progBar Pointer to the progress bar.
 * @param fg Pointer to the foreground texture.
 * @param bg Pointer to the background texture.
 */
CYBAPI void Cyb_SetProgressTextures(Cyb_Grid *progBar, SDL_Texture *fg,
    SDL_Texture *bg);
    
/** @brief Set the maximum progress value for a progress bar.
 *
 * @param progBar Pointer to the progress bar.
 * @param max The new maximum progress value.
 */
CYBAPI void Cyb_SetMaxProgressValue(Cyb_Grid *progBar, int max);

/** @brief Set the current progress value for a progress bar.
 *
 * @param progBar Pointer to the progress bar.
 * @param value The new progress value.
 */
CYBAPI void Cyb_SetProgressValue(Cyb_Grid *progBar, int value);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif