#ifndef CYBQUAT_H
#define CYBQUAT_H

/** @file
 * @brief CybRender - Quaternion API
 */
 
#include "CybCommon.h"
#include "CybMatrix.h"
#include "CybVec.h"

 
#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup CybRender
 * @brief Cybermals Engine - Quaternion API
 * @{
 */

//Functions
//=================================================================================
/** @brief Create a quaternion from an axis and angle.
 *
 * @param quat Pointer to the resulting quaternion.
 * @param x The X component of the axis.
 * @param y The Y component of the axis.
 * @param z The Z component of the axis.
 * @param angle The angle of the rotation.
 */
CYBAPI void Cyb_QuatFromAxisAndAngle(Cyb_Vec4 *quat, float x, float y, float z,
    float angle);
    
/** @brief Multiply 2 quaternions.
 *
 * @param c Pointer to the resulting quaternion.
 * @param a Pointer to the first quaternion.
 * @param b Pointer to the second quaternion.
 */
CYBAPI void Cyb_MulQuat(Cyb_Vec4 *c, const Cyb_Vec4 *a, const Cyb_Vec4 *b);

/** @brief Normalize a quaternion.
 *
 * @param quat Pointer to the quaternion.
 */
CYBAPI void Cyb_NormalizeQuat(Cyb_Vec4 *quat);

/** @brief Calculate the quaternion that lies between a and b given the progression
 * between the 2 quaternions by using spherical linear interpolation.
 *
 * @param c Pointer to the resulting quaternion.
 * @param a Pointer to the start quaternion.
 * @param b Pointer to the end quaternion.
 * @param progress The amount of progress between a and b (between 0.0 and 1.0).
 */
CYBAPI void Cyb_Slerp(Cyb_Vec4 *c, const Cyb_Vec4 *a, const Cyb_Vec4 *b,
    double progress);

/** @brief Convert a quaternion to a matrix.
 *
 * @param Pointer to the resulting matrix.
 * @param Pointer to the quaternion.
 */
CYBAPI void Cyb_QuatToMatrix(Cyb_Mat4 *mat, const Cyb_Vec4 *quat);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif