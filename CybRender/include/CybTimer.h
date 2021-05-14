#ifndef CYBTIMER_H
#define CYBTIMER_H

/** @file
 * @brief CybRender - Timer API
 */
 
#include "CybCommon.h"

 
#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup CybRender
 * @brief Cybermals Engine - Renderer Subsystem
 * @{
 */

//Functions
//=================================================================================
/** @brief Limit the framerate to the given value.
 *
 * @param fps The target framerate.
 */
CYBAPI void Cyb_NextFrame(int fps);

/** @brief Get the number of milliseconds since the last frame.
 *
 * @return The delta time (in milliseconds).
 */
CYBAPI unsigned int Cyb_GetDeltaTime(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif