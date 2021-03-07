#ifndef CYBUIEVENTS_H
#define CYBUIEVENTS_H

/** @file
 * @brief CybUI - UI Events API
 */
 
#include "CybCommon.h"
 
 
#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup CybUI
 * @brief Cybermals Engine - UI Subsystem
 * @{
 */

//Globals
//=================================================================================
CYBAPI int CYB_UI_MOUSEMOTION;     /**< UI mouse motion event. */
CYBAPI int CYB_UI_MOUSEBUTTONDOWN; /**< UI mouse button down event. */
CYBAPI int CYB_UI_MOUSEBUTTONUP;   /**< UI mouse button up event. */


//Functions
//=================================================================================
/** @brief Initialize the UI events.
 *
 * @return CYB_NO_ERROR on success or CYB_ERROR on failure.
 */
CYBAPI int Cyb_InitUIEvents(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif