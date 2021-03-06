#ifndef CYBUILOADER_H
#define CYBUILOADER_H

/** @file
 * @brief CybUI - UI Loader API
 */
 
#include "CybCommon.h"
#include "CybGrid.h"

 
#ifdef __cplusplus
extern "C" {
#endif

//Macros
//=================================================================================
#define Cyb_LoadUI(filename) Cyb_LoadUIFromRW(SDL_RWFromFile(filename, "r"), TRUE)


//Functions
//=================================================================================
CYBAPI Cyb_Grid *Cyb_LoadUIFromRW(SDL_RWops *file, int doClose);

#ifdef __cplusplus
}
#endif

#endif