#ifndef CYBBONE_H
#define CYBBONE_H

/** @file
 * @brief CybRender - Bone API
 */

#include "CybMath.h"


#ifdef __cplusplus
extern "C" {
#endif
 
//Structures
//=================================================================================
typedef struct
{
    char name[32];
    int parent;
    Cyb_Mat4 matrix;
} Cyb_Bone;


#ifdef __cplusplus
}
#endif

#endif