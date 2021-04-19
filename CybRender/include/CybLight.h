#ifndef CYBLIGHT_H
#define CYBLIGHT_H

/** @file
 * @brief CybRender - Light API
 */
 
#include "CybMath.h"
#include "CybObjects.h"
#include "CybRenderer.h"

 
#ifdef __cplusplus
extern "C" {
#endif

//Structures
//=================================================================================
typedef struct
{
    Cyb_Object base;
    Cyb_Vec3 color;
    float ambientStrength;
    float specularStrength;
} Cyb_Light;


//Functions
//=================================================================================
CYBAPI Cyb_Light *Cyb_CreateLight(void);

#ifdef __cplusplus
}
#endif

#endif