#ifndef CYBMATERIAL_H
#define CYBMATERIAL_H

/** @file
 * @brief CybRender - Material API
 */
 
#include "CybMath.h"
#include "CybObjects.h"
 
 
#ifdef __cplusplus
extern "C" {
#endif

//Structures
//=================================================================================
typedef struct
{
    Cyb_Object base;
    Cyb_Vec3 ambient;
    Cyb_Vec3 diffuse;
    Cyb_Vec3 specular;
    float shininess;
} Cyb_Material;


//Functions
//=================================================================================
CYBAPI Cyb_Material *Cyb_CreateMaterial(void);

#ifdef __cplusplus
}
#endif

#endif