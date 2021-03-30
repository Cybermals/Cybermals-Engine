#ifndef CYBVECTOR_H
#define CYBVECTOR_H

/** @file
 * @brief CybMath - Vector API
 */
 
#include "CybCommon.h"
 
 
#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup CybMath
 * @brief Cybermals Engine - 3D Math API
 * @{
 */

//Structures
//=================================================================================
/** @brief A 2D vector.
 */
typedef struct
{
    float x; /**< The X component. */
    float y; /**< The Y component. */
} Cyb_Vec2;


/** @brief A 3D vector.
 */
typedef struct
{
    float x; /**< The X component. */
    float y; /**< The Y component. */
    float z; /**< The Z component. */
} Cyb_Vec3;


/** @brief A 4D vector.
*/
typedef struct
{
    float x; /**< The X component. */
    float y; /**< The Y component. */
    float z; /**< The Z component. */
    float w; /**< The W component. */
} Cyb_Vec4;


//Functions
//=================================================================================
/** @brief Add two 2D vectors.
 *
 * @param c Pointer to the resulting vector.
 * @param a Pointer to the first operand.
 * @param b Pointer to the second operand.
 */
CYBAPI void Cyb_AddVec2(Cyb_Vec2 *c, const Cyb_Vec2 *a, const Cyb_Vec2 *b);

/** @brief Subtract two 2D vectors.
 *
 * @param c Pointer to the resulting vector.
 * @param a Pointer to the first operand.
 * @param b Pointer to the second operand.
 */
CYBAPI void Cyb_SubVec2(Cyb_Vec2 *c, const Cyb_Vec2 *a, const Cyb_Vec2 *b);

/** @brief Multiply two 2D vectors.
 *
 * @param c Pointer to the resulting vector.
 * @param a Pointer to the first operand.
 * @param b Pointer to the second operand.
 */
CYBAPI void Cyb_MulVec2(Cyb_Vec2 *c, const Cyb_Vec2 *a, const Cyb_Vec2 *b);

/** @brief Divide two 2D vectors.
 *
 * @param c Pointer to the resulting vector.
 * @param a Pointer to the first operand.
 * @param b Pointer to the second operand.
 */
CYBAPI void Cyb_DivVec2(Cyb_Vec2 *c, const Cyb_Vec2 *a, const Cyb_Vec2 *b);

/** @brief Add two 3D vectors.
 *
 * @param c Pointer to the resulting vector.
 * @param a Pointer to the first operand.
 * @param b Pointer to the second operand.
 */
CYBAPI void Cyb_AddVec3(Cyb_Vec3 *c, const Cyb_Vec3 *a, const Cyb_Vec3 *b);

/** @brief Subtract two 3D vectors.
 *
 * @param c Pointer to the resulting vector.
 * @param a Pointer to the first operand.
 * @param b Pointer to the second operand.
 */
CYBAPI void Cyb_SubVec3(Cyb_Vec3 *c, const Cyb_Vec3 *a, const Cyb_Vec3 *b);

/** @brief Multiply two 3D vectors.
 *
 * @param c Pointer to the resulting vector.
 * @param a Pointer to the first operand.
 * @param b Pointer to the second operand.
 */
CYBAPI void Cyb_MulVec3(Cyb_Vec3 *c, const Cyb_Vec3 *a, const Cyb_Vec3 *b);

/** @brief Divide two 3D vectors.
 *
 * @param c Pointer to the resulting vector.
 * @param a Pointer to the first operand.
 * @param b Pointer to the second operand.
 */
CYBAPI void Cyb_DivVec3(Cyb_Vec3 *c, const Cyb_Vec3 *a, const Cyb_Vec3 *b);

/** @brief Add two 4D vectors.
 *
 * @param c Pointer to the resulting vector.
 * @param a Pointer to the first operand.
 * @param b Pointer to the second operand.
 */
CYBAPI void Cyb_AddVec4(Cyb_Vec4 *c, const Cyb_Vec4 *a, const Cyb_Vec4 *b);

/** @brief Subtract two 4D vectors.
 *
 * @param c Pointer to the resulting vector.
 * @param a Pointer to the first operand.
 * @param b Pointer to the second operand.
 */
CYBAPI void Cyb_SubVec4(Cyb_Vec4 *c, const Cyb_Vec4 *a, const Cyb_Vec4 *b);

/** @brief Multiply two 4D vectors.
 *
 * @param c Pointer to the resulting vector.
 * @param a Pointer to the first operand.
 * @param b Pointer to the second operand.
 */
CYBAPI void Cyb_MulVec4(Cyb_Vec4 *c, const Cyb_Vec4 *a, const Cyb_Vec4 *b);

/** @brief Divide two 4D vectors.
 *
 * @param c Pointer to the resulting vector.
 * @param a Pointer to the first operand.
 * @param b Pointer to the second operand.
 */
CYBAPI void Cyb_DivVec4(Cyb_Vec4 *c, const Cyb_Vec4 *a, const Cyb_Vec4 *b);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif