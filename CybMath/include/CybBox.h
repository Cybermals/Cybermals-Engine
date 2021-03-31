#ifndef CYBBOX_H
#define CYBBOX_H

/** @file
 * @brief CybMath - Box API
 */
 
#include "CybCommon.h"
#include "CybVector.h"

#ifndef min
#define min(a, b) (a < b ? a : b)
#define max(a, b) (a > b ? a : b)
#endif

 
#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup CybMath
 * @brief Cybermals Engine - 3D Math Library
 * @{
 */

//Structures
//=================================================================================
/** @brief Axis-aligned bounding box (AABB).
 */
typedef struct
{
    Cyb_Vec3 center; /**< The center of the box. */
    Cyb_Vec3 size;   /**< The size of the box. */
} Cyb_Box;


//Functions
//=================================================================================
/** @brief Calculate the smallest box that some given geometry will fit into.
 *
 * @param box Pointer to the resulting box.
 * @param verts Pointer to some vertex data.
 * @param stride The size of each vertex.
 * @param count The number of vertices.
 */
CYBAPI void Cyb_BoxFromGeometry(Cyb_Box *box, void *verts, int stride, int count);

/** @brief Test if a given point lies within a given box.
 *
 * @param a Pointer to the point.
 * @param b Pointer to the box.
 */
CYBAPI int Cyb_PointInBox(const Cyb_Vec3 *a, const Cyb_Box *b);

/** @brief Test if 2 boxes are intersecting.
 *
 * @param a Pointer to the first box.
 * @param b Pointer to the second box.
 *
 * @return TRUE if the boxes are intersecting.
 */
CYBAPI int Cyb_HasIntersection(const Cyb_Box *a, const Cyb_Box *b);

/** @brief Generate geometry data for a given box.
 *
 * @param verts Pointer to the resulting vertices.
 * @param indices Pointer to the resulting indices (can be NULL).
 * @param box Pointer to the box.
 */
CYBAPI void Cyb_GenerateBoxGeometry(Cyb_Vec3 *verts, int *indices, 
    const Cyb_Box *box);
    
/** @brief Rotate a given box.
 *
 * @param out Pointer to the resulting box.
 * @param in Pointer to the original box.
 */
CYBAPI void Cyb_RotateBox(Cyb_Box *out, const Cyb_Box *in, 
    float x, float y, float z);
    
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif