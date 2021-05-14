#ifndef CYBMATRIX_H
#define CYBMATRIX_H

/** @file
 * @brief CybMath - Matrix API
 */
 
#include <math.h>
 
#include "CybCommon.h"
#include "CybVec.h"

#define radians(n) ((n) * (M_PI / 180.0f))

 
#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup CybMath
 * @brief Cybermals Engine - 3D Math Library
 * @{
 */
 
//Enums
//=================================================================================
/** @brief Order to apply rotations.
 */
enum Cyb_RotationOrder
{
    CYB_ROT_XYZ, /**< X, Y, Z rotation order. */
    CYB_ROT_ZYX, /**< Z, Y, X rotation order. */
    CYB_ROT_ZXY  /**< Z, X, Y rotation order. */
};

//Structures
//=================================================================================
/** @brief 4 x 4 matrix.
 */
typedef struct
{
    float a, e, i, m;
    float b, f, j, n;
    float c, g, k, o;
    float d, h, l, p;
} Cyb_Mat4;


//Functions
//=================================================================================
/** @brief Multiply two 4 x 4 matrices.
 *
 * @param c Pointer to the resulting matrix.
 * @param a Pointer to the first operand.
 * @param b Pointer to the second operand.
 */
CYBAPI void Cyb_MulMat4(Cyb_Mat4 *c, const Cyb_Mat4 *a, const Cyb_Mat4 *b);

/** @brief Transpose a 4 x 4 matrix.
 *
 * @param out Pointer to the resulting matrix.
 * @param in Pointer to the original matix.
 */
CYBAPI void Cyb_Transpose(Cyb_Mat4 *out, const Cyb_Mat4 *in);

/** @brief Calculate the determinant of a 4 x 4 matrix.
 *
 * @param m Pointer to the matrix.
 *
 * @return The determinant.
 */
CYBAPI float Cyb_Determinant(const Cyb_Mat4 *m);

/** @brief Invert a 4 x 4 matrix.
 *
 * @param out Pointer to the resulting matrix.
 * @param in Pointer to the original matrix.
 */
CYBAPI void Cyb_Invert(Cyb_Mat4 *out, const Cyb_Mat4 *in);

/** @brief Transform a 3D vector.
 *
 * @param c Pointer to the resulting vector.
 * @param a Pointer to the first operand (a 4 x 4 matrix).
 * @param b Pointer to the second operand (a 3D vector).
 */
CYBAPI void Cyb_Transform(Cyb_Vec3 *c, const Cyb_Mat4 *a, const Cyb_Vec3 *b);

/** @brief Generate an identity matrix.
 *
 * @param m Pointer to the resulting matrix.
 */
CYBAPI void Cyb_Identity(Cyb_Mat4 *m);

/** @brief Generate a translation matrix.
 *
 * @param m Pointer to the resulting matrix.
 * @param x The X offset.
 * @param y The Y offset.
 * @param z The Z offset.
 */
CYBAPI void Cyb_Translate(Cyb_Mat4 *m, float x, float y, float z);

/** @brief Generate a rotation matrix.
 *
 * @param m Pointer to the resulting matrix.
 * @param x The X axis angle.
 * @param y The Y axis angle.
 * @param z The Z axis angle.
 * @param rotOrder The order used to apply rotations.
 */
CYBAPI void Cyb_Rotate(Cyb_Mat4 *m, float x, float y, float z, int rotOrder);

/** @brief Generate a scaling matrix.
 *
 * @param m Pointer to the resulting matrix.
 * @param x The X scale.
 * @param y The Y scale.
 * @param z The Z scale.
 */
CYBAPI void Cyb_Scale(Cyb_Mat4 *m, float x, float y, float z);

/** @brief Generate a look at matrix.
 *
 * @param m Pointer to the resulting matrix.
 * @param pos Pointer to the object position (usually a camera object).
 * @param right Pointer to the right vector.
 * @param up Pointer to the up vector.
 * @param dir Pointer to the direction vector.
 */
CYBAPI void Cyb_LookAt(Cyb_Mat4 *m, Cyb_Vec3 *pos, Cyb_Vec3 *right, Cyb_Vec3 *up, 
    Cyb_Vec3 *dir);

/** @brief Generate an orthographic projection matrix.
 *
 * @param m Pointer to the resulting matrix.
 * @param left Left side of the viewing volume.
 * @param right Right side of the viewing volume.
 * @param top Top of the viewing volume.
 * @param bottom Bottom of the viewing volume.
 * @param near Front of the viewing volume.
 * @param far Back of the viewing volume.
 */
CYBAPI void Cyb_Ortho(Cyb_Mat4 *m, float left, float right, float top, 
    float bottom, float near, float far);
    
/** @brief Generate a perspective projection matrix.
 *
 * @param m Pointer to the resulting matrix.
 * @param fov The field of view angle.
 * @param aspect The aspect ratio.
 * @param near The front of the frustrum.
 * @param far The back of the frustrum.
 */
CYBAPI void Cyb_Perspective(Cyb_Mat4 *m, float fov, float aspect, float near, 
    float far);

/**
 * @}
 */
    
#ifdef __cplusplus
}
#endif

#endif