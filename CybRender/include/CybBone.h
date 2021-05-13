#ifndef CYBBONE_H
#define CYBBONE_H

/** @file
 * @brief CybRender - Bone API
 */

#include "CybMath.h"


#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup CybRender
 * @brief Cybermals Engine - Renderer Subsystem
 * @{
 */
 
//Structures
//=================================================================================
/** @brief Armature bone.
 */
typedef struct
{
    char name[32];   /**< Bone name. */
    int parent;      /**< Parent bone index. */
    Cyb_Mat4 matrix; /**< Bone matrix. */
} Cyb_Bone;

/**
 * @}
 */


#ifdef __cplusplus
}
#endif

#endif