#ifndef CYBOBJECTS_H
#define CYBOBJECTS_H

/** @file
 * @brief CybObjects - Main API
 */
 
#include "CybCommon.h"
#include "CybObject.h"

 
#ifdef __cplusplus
extern "C" {
#endif

//Functions
//==================================================================================
CYBAPI int Cyb_InitObjects(void);
CYBAPI void Cyb_FiniObjects(void);

#ifdef __cplusplus
}
#endif

#endif