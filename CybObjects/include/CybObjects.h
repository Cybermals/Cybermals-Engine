#ifndef CYBOBJECTS_H
#define CYBOBJECTS_H

/** @file
 * @brief CybObjects - Main API
 */
 
#include "CybCommon.h"
#include "CybList.h"
#include "CybObject.h"
#include "CybQueue.h"
#include "CybVector.h"

 
#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup CybObjects
 * @brief Cybermals Engine - Objects API
 * @{
 */

//Functions
//==================================================================================
/** @brief Get the Cybermals Engine version.
 *
 * @return The version as a float.
 */
CYBAPI float Cyb_GetVersion(void);

/** @brief Get the Cybermals Engine version string.
 *
 * @return The version as a string.
 */
CYBAPI const char *Cyb_GetVersionString(void);

/** @brief Initialize the objects subsystem.
 * 
 * @return CYB_NO_ERROR on success or CYB_ERROR on failure.
 */
CYBAPI int Cyb_InitObjects(void);

/** @brief Finialize the objects subsystem.
 */
CYBAPI void Cyb_FiniObjects(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif