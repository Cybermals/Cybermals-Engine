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

/** @addtogroup CybRender
 * @brief Cybermals Engine - Light API
 * @{
 */

//Structures
//=================================================================================
/** @brief A light source.
 */
typedef struct
{
    Cyb_Object base;   /**< Base object. */
    Cyb_Vec3 pos;      /**< Light position. */
    Cyb_Vec3 ambient;  /**< Ambient color. */
    Cyb_Vec3 diffuse;  /**< Diffuse color. */
    Cyb_Vec3 specular; /**< Specular color. */
} Cyb_Light;


//Functions
//=================================================================================
/** @brief Create a new light source.
 *
 * @return Pointer to the light.
 */
CYBAPI Cyb_Light *Cyb_CreateLight(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif