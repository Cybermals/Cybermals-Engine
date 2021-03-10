#ifndef CYBBUTTON_H
#define CYBBUTTON_H

/** @file
 * @brief CybUI - Button API
 */
 
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
/** @brief Button mode enumeration.
 */
enum Cyb_ButtonMode
{
    CYB_BUTTONMODE_PUSH,  /**< Pushbutton mode. */
    CYB_BUTTONMODE_TOGGLE /**< Toggle mode (like a checkbox). */
};


//Functions
//=================================================================================
/** @brief Button drawing procedure.
 *
 * @param button Pointer to the button widget.
 * @param renderer Pointer to the renderer to use.
 */
CYBAPI void Cyb_DrawButtonProc(Cyb_Grid *button, SDL_Renderer *renderer);

/** @brief Button event handler procedure.
 *
 * @param button Pointer to the button widget.
 * @param event Pointer to the event.
 */
CYBAPI void Cyb_HandleButtonEventProc(Cyb_Grid *button, const SDL_Event *event);

/** @brief Create a new button.
 *
 * @return Pointer to the button.
 */
CYBAPI Cyb_Grid *Cyb_CreateButton(void);

/** @brief Set the up and down textures for a given button.
 *
 * @param button Pointer to the button.
 * @param up Pointer to the texture for the button's up state.
 * @param down Pointer to the texture for the button's down state.
 */
CYBAPI void Cyb_SetButtonTextures(Cyb_Grid *button, SDL_Texture *up, 
    SDL_Texture *down);
    
/** @brief Set the mode of a given button.
 *
 * @param button Pointer to the button.
 * @param mode The new button mode.
 */
CYBAPI void Cyb_SetButtonMode(Cyb_Grid *button, int mode);

/** @brief Get the current state of a given button.
 *
 * @param button Pointer to the button.
 *
 * @return TRUE for pressed or FALSE for not pressed.
 */
CYBAPI int Cyb_GetButtonState(Cyb_Grid *button);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif