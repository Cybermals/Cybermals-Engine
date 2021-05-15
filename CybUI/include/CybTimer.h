#ifndef CYBTIMER_H
#define CYBTIMER_H

/** @file
 * @brief CybUI - Timer API
 */
 
#include "CybCommon.h"

 
#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup CybUI
 * @brief Cybermals Engine - UI Subsystem
 * @{
 */

//Functions
//=================================================================================
/** @brief Limit the framerate to the given value.
 *
 * @param fps The target framerate.
 */
CYBAPI void Cyb_NextFrame(int fps);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif