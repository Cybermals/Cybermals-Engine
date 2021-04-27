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

/** @addtogroup CybRender
 * @brief Cybermals Engine - Material API
 * @{
 */

//Structures
//=================================================================================
/** @brief A material.
 */
typedef struct
{
    Cyb_Object base;   /**< Base object. */
    Cyb_Vec3 ambient;  /**< Ambient color. */
    Cyb_Vec3 diffuse;  /**< Diffuse color. */
    Cyb_Vec3 specular; /**< Specular color. */
    float shininess;   /**< Shininess. */
} Cyb_Material;


//Functions
//=================================================================================
/** @brief Create a new material.
 *
 * @return Pointer to the material.
 */
CYBAPI Cyb_Material *Cyb_CreateMaterial(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif