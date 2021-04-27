#ifndef CYBARMATURE_H
#define CYBARMATURE_H

/** @file
 * @brief CybRender - Armature API
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
    char name[32];
    Cyb_Mat4 matrix;
} Cyb_Bone;

#ifdef __cplusplus
}
#endif

#endif