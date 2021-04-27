/*
CybRender - Quaternion API
*/

#include "CybQuat.h"


//Functions
//=================================================================================
void Cyb_QuatFromAxisAndAngle(Cyb_Vec4 *quat, float x, float y, float z,
    float angle)
{
    quat->x = x * sinf(radians(angle) / 2.0f);
    quat->y = y * sinf(radians(angle) / 2.0f);
    quat->z = z * sinf(radians(angle) / 2.0f);
    quat->w = cosf(radians(angle) / 2.0f);
}


void Cyb_MulQuat(Cyb_Vec4 *c, const Cyb_Vec4 *a, const Cyb_Vec4 *b)
{
    c->x = a->w * b->x + a->x * b->w + a->y * b->z - a->z * b->y;
    c->y = a->w * b->y - a->x * b->z + a->y * b->w + a->z * b->x;
    c->z = a->w * b->z + a->x * b->y - a->y * b->x + a->z * b->w;
    c->w = a->w * b->w - a->x * b->x - a->y * b->y - a->z * b->z;
}


void Cyb_NormalizeQuat(Cyb_Vec4 *quat)
{
    float magnitude = sqrt(quat->x * quat->x + quat->y * quat->y + quat->z * quat->z + quat->w * quat->w);
    quat->x /= magnitude;
    quat->y /= magnitude;
    quat->z /= magnitude;
    quat->w /= magnitude;
}


void Cyb_QuatToMatrix(Cyb_Mat4 *mat, const Cyb_Vec4 *quat)
{
    //Start with the identity matrix
    Cyb_Identity(mat);
    
    //Row 1
    mat->a = 1.0f - 2.0f * (quat->y * quat->y + quat->z * quat->z);
    mat->b = 2.0f * (quat->x * quat->y - quat->w * quat->z);
    mat->c = 2.0f * (quat->x * quat->z + quat->w * quat->y);
    
    //Row 2
    mat->e = 2.0f * (quat->x * quat->y + quat->w * quat->z);
    mat->f = 1.0f - 2.0f * (quat->x * quat->x + quat->z * quat->z);
    mat->g = 2.0f * (quat->y * quat->z - quat->w * quat->x);
    
    //Row 3
    mat->i = 2.0f * (quat->x * quat->z - quat->w * quat->y);
    mat->j = 2.0f * (quat->y * quat->z + quat->w * quat->x);
    mat->k = 1.0f - 2.0f * (quat->x * quat->x + quat->y * quat->y);
}