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

/** @addtogroup CybRender
 * @brief Cybermals Engine - Renderer Subsystem
 * @{
 */

//Types
//================================================================================
/** @brief An armature.
 */
typedef struct Cyb_Armature Cyb_Armature;

/** @brief An armature pose.
 */
typedef struct Cyb_Pose Cyb_Pose;


//Structures
//================================================================================
/** @brief Per vertex armature data.
 */
typedef struct
{
    Cyb_Vec4 group;  /**< Vertex groups (up to 4). */
    Cyb_Vec4 weight; /**< Vertex group weights (up to 4). */
} Cyb_VertexGW;


//Functions
//================================================================================
/** @brief Create a new armature.
 *
 * @param renderer Pointer to the renderer.
 *
 * @return Pointer to the armature.
 */
CYBAPI Cyb_Armature *Cyb_CreateArmature(Cyb_Renderer *renderer);

/** @brief Update an armature.
 *
 * @param renderer Pointer to the renderer.
 * @param armature Pointer to the armature.
 * @param vertCount The number of vertices (must match the vertex count of the mesh).
 * @param vgroups The vertex groups.
 * @param vweights The vertex group weights.
 * @param boneCount The number of bones.
 * @param bones The bones that make up the armature.
 */
CYBAPI void Cyb_UpdateArmature(Cyb_Renderer *renderer, Cyb_Armature *armature, 
    int vertCount, const Cyb_Vec4 *vgroups, const Cyb_Vec4 *vweights, 
    int boneCount, const Cyb_Bone *bones);
    
/** @brief Create a new armature pose.
 *
 * @param armature Pointer to the armature.
 *
 * @return Pointer to the pose.
 */
CYBAPI Cyb_Pose *Cyb_CreatePose(Cyb_Armature *armature);

/** @brief Get the ID of a bone.
 *
 * @param pose Pointer to the armature pose.
 * @param name The name of the bone.
 *
 * @return The bone ID.
 */
CYBAPI int Cyb_GetBoneID(Cyb_Pose *pose, const char *name);

/** @brief Update the transformation matrix of an armature bone.
 *
 * @param pose Pointer to the pose.
 * @param boneID Index of the bone.
 * @param matrix Pointer to the new matrix.
 */
CYBAPI void Cyb_UpdateBone(Cyb_Pose *pose, int boneID, const Cyb_Mat4 *matrix);

/** @brief Select the pose to use when rendering a mesh.
 *
 * @param renderer Pointer to the renderer.
 * @param shader Pointer to the shader.
 * @param pose Pointer to the pose.
 */
CYBAPI void Cyb_SelectPose(Cyb_Renderer *renderer, Cyb_Shader *shader, 
    Cyb_Pose *pose);
    
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif