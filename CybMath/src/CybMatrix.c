/*
CybMath - Matrix API
*/

#include <math.h>

#include "CybMatrix.h"


//Functions
//=================================================================================
void Cyb_Identity(Cyb_Mat4 *m)
{}


void Cyb_Translate(Cyb_Mat4 *m, float x, float y, float z)
{}


void Cyb_Rotate(Cyb_Mat4 *m, float x, float y, float z)
{}


void Cyb_Scale(Cyb_Mat4 *m, float x, float y, float z)
{}


void Cyb_Ortho(Cyb_Mat4 *m, float left, float right, float top, 
    float bottom, float near, float far)
{}


void Cyb_Perspective(Cyb_Mat4 *m, float fov, float aspect, float near, 
    float far)
{}


void Cyb_MulMat4(Cyb_Mat4 *c, const Cyb_Mat4 *a, const Cyb_Mat4 *b)
{}