#ifndef CYBKEYFRAMES_H
#define CYBKEYFRAMES_H

/** @file
 * @brief CybRender - Keyframes API
 */
 
#include "CybMath.h"

 
//Structures
//=================================================================================
typedef struct
{
    double time;
    Cyb_Vec3 value;
} Cyb_VecKey;


typedef struct
{
    double time;
    Cyb_Vec4 value;
} Cyb_QuatKey;

#endif