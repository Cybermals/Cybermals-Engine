#ifndef CYBARMATURE_H
#define CYBARMATURE_H

/** @file
 * @brief CybRender - Armature API
 */
 
#include "CybBone.h"
#include "CybMath.h"
#include "CybObjects.h"
#include "CybRenderer.h"

 
#ifdef __cplusplus
extern "C" {
#endif

//Types
//================================================================================
typedef struct Cyb_Armature Cyb_Armature;


//Structures
//================================================================================
typedef struct
{
    Cyb_Vec4 group;
    Cyb_Vec4 weight;
} Cyb_VertexGW;


typedef struct
{
    Cyb_Object base;
    int boneCount;
    Cyb_Mat4 *matrices;
    Cyb_Mat4 *bones;
} Cyb_Pose;


//Functions
//================================================================================
CYBAPI Cyb_Armature *Cyb_CreateArmature(Cyb_Renderer *renderer);
CYBAPI void Cyb_UpdateArmature(Cyb_Renderer *renderer, Cyb_Armature *armature, 
    int vertCount, const Cyb_Vec4 *vgroups, const Cyb_Vec4 *vweights, 
    int boneCount, const Cyb_Bone *bones);
CYBAPI Cyb_Pose *Cyb_CreatePose(Cyb_Armature *armature);
CYBAPI void Cyb_UpdateBone(Cyb_Armature *armature, Cyb_Pose *pose, int bone, 
    const Cyb_Mat4 *matrix);

#ifdef __cplusplus
}
#endif

#endif