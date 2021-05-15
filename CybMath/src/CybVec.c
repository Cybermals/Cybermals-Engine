/*
CybMath - Mathematical Vector API
*/

#include "CybVec.h"


//Functions
//=================================================================================
void Cyb_AddVec2(Cyb_Vec2 *c, const Cyb_Vec2 *a, const Cyb_Vec2 *b)
{
    c->x = a->x + b->x;
    c->y = a->y + b->y;
}


void Cyb_SubVec2(Cyb_Vec2 *c, const Cyb_Vec2 *a, const Cyb_Vec2 *b)
{
    c->x = a->x - b->x;
    c->y = a->y - b->y;
}


void Cyb_MulVec2(Cyb_Vec2 *c, const Cyb_Vec2 *a, const Cyb_Vec2 *b)
{
    c->x = a->x * b->x;
    c->y = a->y * b->y;
}


void Cyb_DivVec2(Cyb_Vec2 *c, const Cyb_Vec2 *a, const Cyb_Vec2 *b)
{
    c->x = a->x / b->x;
    c->y = a->y / b->y;
}


void Cyb_AddVec3(Cyb_Vec3 *c, const Cyb_Vec3 *a, const Cyb_Vec3 *b)
{
    c->x = a->x + b->x;
    c->y = a->y + b->y;
    c->z = a->z + b->z;
}


void Cyb_SubVec3(Cyb_Vec3 *c, const Cyb_Vec3 *a, const Cyb_Vec3 *b)
{
    c->x = a->x - b->x;
    c->y = a->y - b->y;
    c->z = a->z - b->z;
}


void Cyb_MulVec3(Cyb_Vec3 *c, const Cyb_Vec3 *a, const Cyb_Vec3 *b)
{
    c->x = a->x * b->x;
    c->y = a->y * b->y;
    c->z = a->z * b->z;
}


void Cyb_DivVec3(Cyb_Vec3 *c, const Cyb_Vec3 *a, const Cyb_Vec3 *b)
{
    c->x = a->x / b->x;
    c->y = a->y / b->y;
    c->z = a->z / b->z;
}


void Cyb_Lerp(Cyb_Vec3 *c, const Cyb_Vec3 *a, const Cyb_Vec3 *b,
    double progress)
{
    //Calculate increment
    Cyb_Vec3 inc;
    Cyb_SubVec3(&inc, b, a);
    inc.x *= progress;
    inc.y *= progress;
    inc.z *= progress;
    
    //Calculate current position
    Cyb_AddVec3(c, a, &inc);
}


void Cyb_AddVec4(Cyb_Vec4 *c, const Cyb_Vec4 *a, const Cyb_Vec4 *b)
{
    c->x = a->x + b->x;
    c->y = a->y + b->y;
    c->z = a->z + b->z;
    c->w = a->w + b->w;
}


void Cyb_SubVec4(Cyb_Vec4 *c, const Cyb_Vec4 *a, const Cyb_Vec4 *b)
{
    c->x = a->x - b->x;
    c->y = a->y - b->y;
    c->z = a->z - b->z;
    c->w = a->w - b->w;
}


void Cyb_MulVec4(Cyb_Vec4 *c, const Cyb_Vec4 *a, const Cyb_Vec4 *b)
{
    c->x = a->x * b->x;
    c->y = a->y * b->y;
    c->z = a->z * b->z;
    c->w = a->w * b->w;
}


void Cyb_DivVec4(Cyb_Vec4 *c, const Cyb_Vec4 *a, const Cyb_Vec4 *b)
{
    c->x = a->x / b->x;
    c->y = a->y / b->y;
    c->z = a->z / b->z;
    c->w = a->w / b->w;
}