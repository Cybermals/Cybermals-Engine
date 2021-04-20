/*
CybRender - Camera API
*/

#include <math.h>

#include "CybCamera.h"
#include "CybObjects.h"


//Structures
//=================================================================================
struct Cyb_Camera
{
    Cyb_Object base;
    Cyb_Vec3 pos;
    Cyb_Vec3 rot;
    float zoom;
    int isDirty;
    Cyb_Mat4 viewMat;
};


//Functions
//=================================================================================
Cyb_Camera *Cyb_CreateCamera(void)
{
    //Allocate a new camera
    Cyb_Camera *cam = (Cyb_Camera*)Cyb_CreateObject(sizeof(Cyb_Camera), NULL,
        CYB_CAMERA);
        
    if(!cam)
    {
        return NULL;
    }
    
    //Initialize the camera
    cam->pos.x = 0.0f;
    cam->pos.y = 0.0f;
    cam->pos.z = 0.0f;
    cam->rot.x = 0.0f;
    cam->rot.y = 0.0f;
    cam->rot.z = 0.0f;
    cam->zoom = 1.0f;
    cam->isDirty = FALSE;
    Cyb_Identity(&cam->viewMat);
    return cam;
}


void Cyb_SetCameraPos(Cyb_Camera *cam, float x, float y, float z)
{
    cam->pos.x = x;
    cam->pos.y = y;
    cam->pos.z = z;
    cam->isDirty = TRUE;
}


const Cyb_Vec3 *Cyb_GetCameraPos(Cyb_Camera *cam)
{
    return &cam->pos;
}


void Cyb_SetCameraRot(Cyb_Camera *cam, float x, float y, float z)
{
    cam->rot.x = x;
    cam->rot.y = y;
    cam->rot.z = z;
    cam->isDirty = TRUE;
}


const Cyb_Vec3 *Cyb_GetCameraRot(Cyb_Camera *cam)
{
    return &cam->rot;
}


void Cyb_SetCameraZoom(Cyb_Camera *cam, float zoom)
{
    cam->zoom = zoom;
    cam->isDirty = TRUE;
}


float Cyb_GetCameraZoom(Cyb_Camera *cam)
{
    return cam->zoom;
}


void Cyb_MoveCamera(Cyb_Camera *cam, float velocity)
{
    //Ignore velocity of 0
    if(velocity == 0.0f)
    {
        return;
    }

    //Update camera pos
    cam->pos.x += -velocity * sinf(radians(cam->rot.y));
    cam->pos.z += -velocity * cosf(radians(cam->rot.y));
    cam->isDirty = TRUE;
}


void Cyb_StrafeCamera(Cyb_Camera *cam, float velocity)
{
    //Ignore velocity of 0
    if(velocity == 0.0f)
    {
        return;
    }
    
    //Update camera pos
    float theta = cam->rot.y + 90.0f;
    cam->pos.x += -velocity * sinf(radians(theta));
    cam->pos.z += -velocity * cosf(radians(theta));
    cam->isDirty = TRUE;
}


void Cyb_RotateCamera(Cyb_Camera *cam, float x, float y, float z)
{
    //Ignore 0 rotation
    if(x == 0.0f && y == 0.0f && z == 0.0f)
    {
        return;
    }
    
    //Update camera rotation
    cam->rot.x += x;
    cam->rot.y += y;
    cam->rot.z += z;
    cam->isDirty = TRUE;
}


void Cyb_AimCamera(Cyb_Camera *cam, Cyb_Vec3 *pos, Cyb_Vec3 *right,
    Cyb_Vec3 *up, Cyb_Vec3 *dir)
{
    //Update matrix and force dirty flag off
    Cyb_LookAt(&cam->viewMat, pos, right, up, dir);
    cam->isDirty = FALSE;
}


Cyb_Mat4 *Cyb_GetViewMatrix(Cyb_Camera *cam)
{
    //Only update the view matrix if the camera has moved
    if(cam->isDirty)
    {
        Cyb_Mat4 t;
        Cyb_Mat4 r;
        Cyb_Mat4 s;
        Cyb_Mat4 sr;
        
        Cyb_Translate(&t, -cam->pos.x, -cam->pos.y, -cam->pos.z);
        Cyb_Rotate(&r, -cam->rot.x, -cam->rot.y, -cam->rot.z);
        Cyb_Scale(&s, cam->zoom, cam->zoom, cam->zoom);
        Cyb_MulMat4(&sr, &s, &r);
        Cyb_MulMat4(&cam->viewMat, &sr, &t);
        
        cam->isDirty = FALSE;
    }
    
    return &cam->viewMat;
}