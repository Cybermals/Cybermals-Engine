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
    Cyb_Vec3 pos;
    Cyb_Vec3 ambient;
    Cyb_Vec3 diffuse;
    Cyb_Vec3 specular;
} Cyb_Light;


//Functions
//=================================================================================
CYBAPI Cyb_Light *Cyb_CreateLight(void);

#ifdef __cplusplus
}
#endif

#endif