#ifndef CYBMATRIX_H
#define CYBMATRIX_H

/** @file
 * @brief CybMath - Matrix API
 */
 
#include "CybCommon.h"

 
#ifdef __cplusplus
extern "C" {
#endif

//Structures
//=================================================================================
typedef struct
{
    float a, e, i, m;
    float b, f, j, n;
    float c, g, k, o;
    float d, h, l, p;
} Cyb_Mat4;


//Functions
//=================================================================================
CYBAPI void Cyb_Identity(Cyb_Mat4 *m);
CYBAPI void Cyb_Translate(Cyb_Mat4 *m, float x, float y, float z);
CYBAPI void Cyb_Rotate(Cyb_Mat4 *m, float x, float y, float z);
CYBAPI void Cyb_Scale(Cyb_Mat4 *m, float x, float y, float z);
CYBAPI void Cyb_Ortho(Cyb_Mat4 *m, float left, float right, float top, 
    float bottom, float near, float far);
CYBAPI void Cyb_Perspective(Cyb_Mat4 *m, float fov, float aspect, float near, 
    float far);
CYBAPI void Cyb_MulMat4(Cyb_Mat4 *c, const Cyb_Mat4 *a, const Cyb_Mat4 *b);

#ifdef __cplusplus
}
#endif

#endif