#ifndef CYBARMATURE_H
#define CYBARMATURE_H

/** @file
 * @brief CybRender - Armature API
 */
 
#include "CybBone.h"
#include "CybMath.h"
#include "CybObjects.h"
#include "CybRenderer.h"
#include "CybShader.h"

 
#ifdef __cplusplus
extern "C" {
#endif

//Types
//================================================================================
typedef struct Cyb_Armature Cyb_Armature;
typedef struct Cyb_Pose Cyb_Pose;


//Structures
//================================================================================
typedef struct
{
    Cyb_Vec4 group;
    Cyb_Vec4 weight;
} Cyb_VertexGW;


//Functions
//================================================================================
CYBAPI Cyb_Armature *Cyb_CreateArmature(Cyb_Renderer *renderer);
CYBAPI void Cyb_UpdateArmature(Cyb_Renderer *renderer, Cyb_Armature *armature, 
    int vertCount, const Cyb_Vec4 *vgroups, const Cyb_Vec4 *vweights, 
    int boneCount, const Cyb_Bone *bones);
CYBAPI Cyb_Pose *Cyb_CreatePose(Cyb_Armature *armature);
CYBAPI void Cyb_UpdateBone(Cyb_Pose *pose, int boneID, const Cyb_Mat4 *matrix);
CYBAPI void Cyb_SelectPose(Cyb_Renderer *renderer, Cyb_Shader *shader, 
    Cyb_Pose *pose);

#ifdef __cplusplus
}
#endif

#endif