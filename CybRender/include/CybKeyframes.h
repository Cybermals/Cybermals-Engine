#ifndef CYBKEYFRAMES_H
#define CYBKEYFRAMES_H

/** @file
 * @brief CybRender - Keyframes API
 */
 
#include "CybMath.h"


/** @addtogroup CybRender
 * @brief Cybermals Engine - Renderer Subsystem
 * @{
 */

 
//Structures
//=================================================================================
/** @brief Vector keyframe.
 */
typedef struct
{
    double time;    /**< Timestamp. */
    Cyb_Vec3 value; /**< Value. */
} Cyb_VecKey;


/** @brief Quaternion keyframe.
 */
typedef struct
{
    double time;    /**< Timestamp. */
    Cyb_Vec4 value; /**< Value. */
} Cyb_QuatKey;

/**
 * @}
 */

#endif