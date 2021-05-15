#ifndef CYBSPHERE_H
#define CYBSPHERE_H

/** @file
 * @brief CybMath - Sphere API
 */
 
#include "CybCommon.h"
#include "CybVec.h"

 
#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup CybMath
 * @brief Cybermals Engine - 3D Math Library
 */

//Structures
//=================================================================================
/** @brief A sphere.
 */
typedef struct
{
    Cyb_Vec3 center; /**< Sphere center. */
    float radius;    /**< Sphere radius. */
} Cyb_Sphere;


//Functions
//=================================================================================
/** @brief Test if a given point is inside a given sphere.
 *
 * @param a Pointer to the point.
 * @param b Pointer to the sphere.
 *
 * @return TRUE if the point is inside the sphere.
 */
CYBAPI int Cyb_PointInSphere(const Cyb_Vec3 *a, const Cyb_Sphere *b);

/** @brief Test if 2 spheres are intersecting.
 *
 * @param a The first sphere.
 * @param b The second sphere.
 *
 * @return TRUE if the spheres are intersecting.
 */
CYBAPI int Cyb_SphereHitSphere(const Cyb_Sphere *a, const Cyb_Sphere *b);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif