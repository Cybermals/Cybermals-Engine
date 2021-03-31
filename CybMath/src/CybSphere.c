/*
CybMath - Sphere API
*/

#include <math.h>

#include "CybSphere.h"


//Functions
//=================================================================================
int Cyb_PointInSphere(const Cyb_Vec3 *a, const Cyb_Sphere *b)
{
    float xdist = fabs(b->center.x - a->x);
    float ydist = fabs(b->center.y - a->y);
    float zdist = fabs(b->center.z - a->z);
    return (sqrt(xdist * xdist + ydist * ydist + zdist * zdist) < b->radius);
}


int Cyb_SphereHitSphere(const Cyb_Sphere *a, const Cyb_Sphere *b)
{
    float xdist = fabs(b->center.x - a->center.x);
    float ydist = fabs(b->center.y - a->center.y);
    float zdist = fabs(b->center.z - a->center.z);
    return (sqrt(xdist * xdist + ydist * ydist + zdist * zdist) < 
        a->radius + b->radius);
}