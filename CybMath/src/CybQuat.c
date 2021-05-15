/*
CybRender - Quaternion API
*/

#include <string.h>

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


void Cyb_Slerp(Cyb_Vec4 *c, const Cyb_Vec4 *a, const Cyb_Vec4 *b,
    double progress)
{
    //Calculate angle between a and b
    double cosHalfTheta = a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w;
    
    if(fabs(cosHalfTheta) >= 1.0)
    {
        memcpy(c, a, sizeof(Cyb_Vec4));
        return;
    }
    
    //Follow shortest path
    int reverseB = FALSE;
    
    if(cosHalfTheta < 0)
    {
        reverseB = TRUE;
        cosHalfTheta = -cosHalfTheta;
    }
    
    //Calculate temp values
    const double halfTheta = acos(cosHalfTheta);
    const double sinHalfTheta = sqrt(1.0 - (cosHalfTheta * cosHalfTheta));
    
    //If theta is 180 degrees, then the result is not fully defined since we could
    //rotate either direction to get there
    if(fabs(sinHalfTheta) < .001)
    {
        if(!reverseB)
        {
            c->x = (1.0 - progress) * a->x + progress * b->x;
            c->y = (1.0 - progress) * a->y + progress * b->y;
            c->z = (1.0 - progress) * a->z + progress * b->z;
            c->w = (1.0 - progress) * a->w + progress * b->w;
        }
        else
        {
            c->x = (1.0 - progress) * a->x - progress * b->x;
            c->y = (1.0 - progress) * a->y - progress * b->y;
            c->z = (1.0 - progress) * a->z - progress * b->z;
            c->w = (1.0 - progress) * a->w - progress * b->w;
        }
        
        return;
    }
    
    //All other cases
    const double tmp1 = sin((1.0 - progress) * halfTheta) / sinHalfTheta;
    const double tmp2 = sin(progress * halfTheta) / sinHalfTheta;
    
    if(!reverseB)
    {
        c->x = tmp1 * a->x + tmp2 * b->x;
        c->y = tmp1 * a->y + tmp2 * b->y;
        c->z = tmp1 * a->z + tmp2 * b->z;
        c->w = tmp1 * a->w + tmp2 * b->w;
    }
    else
    {
        c->x = tmp1 * a->x - tmp2 * b->x;
        c->y = tmp1 * a->y - tmp2 * b->y;
        c->z = tmp1 * a->z - tmp2 * b->z;
        c->w = tmp1 * a->w - tmp2 * b->w;
    }
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