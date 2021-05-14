/*
CybRender - Material API
*/

#include "CybMaterial.h"


//Functions
//=================================================================================
Cyb_Material *Cyb_CreateMaterial(void)
{
    //Allocate new material
    Cyb_Material *mat = (Cyb_Material*)Cyb_CreateObject(sizeof(Cyb_Material), NULL,
        CYB_MATERIAL);
        
    if(!mat)
    {
        return NULL;
    }
    
    //Initialize the material
    mat->ambient.x = 1.0f;
    mat->ambient.y = 1.0f;
    mat->ambient.z = 1.0f;
    mat->diffuse.x = 1.0f;
    mat->diffuse.y = 1.0f;
    mat->diffuse.z = 1.0f;
    mat->specular.x = 1.0f;
    mat->specular.y = 1.0f;
    mat->specular.z = 1.0f;
    mat->shininess = 32.0f;
    return mat;
}