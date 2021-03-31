/*
CybMath - Box API
*/

#include <math.h>
#include <string.h>

#include "CybBox.h"
#include "CybMatrix.h"


//Functions
//=================================================================================
void Cyb_BoxFromGeometry(Cyb_Box *box, void *verts, int stride, int count)
{
    //Calculate minimum and maximum x, y, and z values
    float xmin = 0;
    float xmax = 0;
    float ymin = 0;
    float ymax = 0;
    float zmin = 0;
    float zmax = 0;
    Cyb_Vec3 *vert = (Cyb_Vec3*)verts;
    
    for(int i = 0; i < count; i++)
    {
        //Process next vertex
        xmin = min(xmin, vert->x);
        xmax = max(xmax, vert->x);
        ymin = min(ymin, vert->y);
        ymax = max(ymax, vert->y);
        zmin = min(zmin, vert->z);
        zmax = max(zmax, vert->z);
        vert = (Cyb_Vec3*)((char*)vert + stride);
    }
    
    //Calculate size and center
    box->size.x = xmax - xmin;
    box->size.y = ymax - ymin;
    box->size.z = zmax - zmin;
    box->center.x = xmin + box->size.x / 2.0f;
    box->center.y = ymin + box->size.y / 2.0f;
    box->center.z = zmin + box->size.z / 2.0f;
}


int Cyb_PointInBox(const Cyb_Vec3 *a, const Cyb_Box *b)
{
    return (a->x > b->center.x - b->size.x / 2.0f &&
        a->x < b->center.x + b->size.x / 2.0f &&
        a->y > b->center.y - b->size.y / 2.0f &&
        a->y < b->center.y + b->size.y / 2.0f &&
        a->z > b->center.z - b->size.z / 2.0f &&
        a->z < b->center.z + b->size.z / 2.0f);
}


int Cyb_BoxHitBox(const Cyb_Box *a, const Cyb_Box *b)
{
    return (fabs(b->center.x - a->center.x) < (a->size.x + b->size.x) / 2.0f &&
        fabs(b->center.y - a->center.y) < (a->size.y + b->size.y) / 2.0f &&
        fabs(b->center.z - a->center.z) < (a->size.z + b->size.z) / 2.0f);
}


void Cyb_GenerateBoxGeometry(Cyb_Vec3 *verts, int *indices, 
    const Cyb_Box *box)
{
    //Calculate left, right, top, bottom, front, and back planes
    float left = box->center.x - box->size.x / 2.0f;
    float right = box->center.x + box->size.x / 2.0f;
    float top = box->center.y + box->size.y / 2.0f;
    float bottom = box->center.y - box->size.y / 2.0f;
    float front = box->center.z + box->size.z / 2.0f;
    float back = box->center.z - box->size.z / 2.0f;
    
    //Generate vertices
    verts[0].x = left;
    verts[0].y = top;
    verts[0].z = front;
    verts[1].x = right;
    verts[1].y = top;
    verts[1].z = front;
    verts[2].x = left;
    verts[2].y = bottom;
    verts[2].z = front;
    verts[3].x = right;
    verts[3].y = bottom;
    verts[3].y = front;
    
    verts[4].x = left;
    verts[4].y = top;
    verts[4].z = back;
    verts[5].x = right;
    verts[5].y = top;
    verts[5].z = back;
    verts[6].x = left;
    verts[6].y = bottom;
    verts[6].z = back;
    verts[7].x = right;
    verts[7].y = bottom;
    verts[7].y = back;
    
    //Generate indices
    if(!indices)
    {
        return;
    }
    
    indices[0] = 0;
    indices[1] = 2;
    indices[2] = 3;
    indices[3] = 3;
    indices[4] = 1;
    indices[5] = 0;
    
    indices[6] = 5;
    indices[7] = 7;
    indices[8] = 6;
    indices[9] = 6;
    indices[10] = 4;
    indices[11] = 5;
    
    indices[12] = 4;
    indices[13] = 6;
    indices[14] = 2;
    indices[15] = 2;
    indices[16] = 0;
    indices[17] = 4;
    
    indices[18] = 1;
    indices[19] = 3;
    indices[20] = 7;
    indices[21] = 7;
    indices[22] = 5;
    indices[23] = 1;
    
    indices[24] = 4;
    indices[25] = 0;
    indices[26] = 1;
    indices[27] = 1;
    indices[28] = 5;
    indices[29] = 4;
    
    indices[30] = 2;
    indices[31] = 6;
    indices[32] = 7;
    indices[33] = 7;
    indices[34] = 3;
    indices[35] = 2;
}


void Cyb_RotateBox(Cyb_Box *out, const Cyb_Box *in, 
    float x, float y, float z)
{
    //Generate box geometry and rotate it
    Cyb_Vec3 verts[8];
    Cyb_GenerateBoxGeometry(verts, NULL, in);
    Cyb_Mat4 r;
    Cyb_Rotate(&r, x, y, z);
    
    for(int i = 0; i < sizeof(verts) / sizeof(verts[0]); i++)
    {
        //Transform next vertex
        Cyb_Vec3 tmp;
        Cyb_Transform(&tmp, &r, &verts[i]);
        memcpy(&verts[i], &tmp, sizeof(tmp));
    }
    
    //Generate new bounding box
    Cyb_BoxFromGeometry(out, verts, sizeof(Cyb_Vec3), 
        sizeof(verts) / sizeof(verts[0]));
}