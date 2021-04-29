/*
CybRender - Armature API
*/

#include <string.h>

#include "CybArmature.h"


//Structures
//================================================================================
struct Cyb_Armature
{
    Cyb_Object base;
    Cyb_Renderer *renderer;
    GLuint vbo;
    int boneCount;
    Cyb_Bone *bones;
};


//Functions
//================================================================================
static void Cyb_FreeArmature(Cyb_Armature *armature)
{
    //Free the VBO
    if(armature->vbo)
    {
        Cyb_GLExtAPI *glExtAPI = Cyb_GetGLExtAPI(armature->renderer);
        glExtAPI->DeleteBuffers(1, &armature->vbo);
    }
    
    //Free the bone array
    if(armature->bones)
    {
        SDL_free(armature->bones);
    }
}


static void Cyb_FreePose(Cyb_Pose *pose)
{
    //Free pose matrices
    if(pose->matrices)
    {
        SDL_free(pose->matrices);
    }
    
    //Free pose bones
    if(pose->bones)
    {
        SDL_free(pose->bones);
    }
}


Cyb_Armature *Cyb_CreateArmature(Cyb_Renderer *renderer)
{
    //Allocate new armature
    Cyb_Armature *armature = (Cyb_Armature*)Cyb_CreateObject(sizeof(Cyb_Armature),
        (Cyb_FreeProc)&Cyb_FreeArmature, CYB_ARMATURE);
        
    if(!armature)
    {
        return NULL;
    }
    
    //Initialize the armature
    armature->renderer = renderer;
    Cyb_GLExtAPI *glExtAPI = Cyb_GetGLExtAPI(renderer);
    Cyb_SelectRenderer(renderer);
    glExtAPI->GenBuffers(1, &armature->vbo);
    
    if(!armature->vbo)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "[CybRender] Failed to allocate VBO.");
        Cyb_FreeObject((Cyb_Object**)&armature);
        return NULL;
    }
    
    armature->boneCount = 0;
    armature->bones = NULL;
    return armature;
}


void Cyb_UpdateArmature(Cyb_Renderer *renderer, Cyb_Armature *armature, 
    int vertCount, const Cyb_Vec4 *vgroups, const Cyb_Vec4 *vweights, 
    int boneCount, const Cyb_Bone *bones)
{
    //Select the renderer
    Cyb_SelectRenderer(renderer);
    
    //Update VBO
    Cyb_GLExtAPI *glExtAPI = Cyb_GetGLExtAPI(renderer);
    glExtAPI->BindBuffer(GL_ARRAY_BUFFER, armature->vbo);
    glExtAPI->BufferData(GL_ARRAY_BUFFER, sizeof(Cyb_VertexGW) * vertCount, NULL,
        GL_STATIC_DRAW);
    Cyb_VertexGW *vertices = (Cyb_VertexGW*)glExtAPI->MapBufferRange(
        GL_ARRAY_BUFFER, 0, sizeof(Cyb_VertexGW) * vertCount, GL_MAP_WRITE_BIT);
        
    if(!vertices)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "[CybRender] Failed to map VBO.");
        return;
    }
    
    for(int i = 0; i < vertCount; i++)
    {
        memcpy(&vertices[i].group, &vgroups[i], sizeof(Cyb_Vec4));
        memcpy(&vertices[i].weight, &vweights[i], sizeof(Cyb_Vec4));
    }
    
    glExtAPI->UnmapBuffer(GL_ARRAY_BUFFER);
    
    //Copy the bones
    if(armature->bones)
    {
        free(armature->bones);
    }
    
    armature->boneCount = boneCount;
    armature->bones = (Cyb_Bone*)SDL_malloc(sizeof(Cyb_Bone) * boneCount);
    
    if(!armature->bones)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "[CybRender] Out of Memory");
        armature->boneCount = 0;
        return;
    }
    
    memcpy(armature->bones, bones, sizeof(Cyb_Bone) * boneCount);
}


Cyb_Pose *Cyb_CreatePose(Cyb_Armature *armature)
{
    //Allocate new pose
    Cyb_Pose *pose = (Cyb_Pose*)Cyb_CreateObject(sizeof(Cyb_Pose),
        (Cyb_FreeProc)&Cyb_FreePose, CYB_POSE);
        
    if(!pose)
    {
        return NULL;
    }
    
    //Initialize the pose
    pose->boneCount = armature->boneCount;
    pose->matrices = (Cyb_Mat4*)SDL_malloc(sizeof(Cyb_Mat4) * pose->boneCount);
    
    if(!pose->matrices)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "[CybRender] Out of Memory");
        Cyb_FreeObject((Cyb_Object**)&pose);
        return NULL;
    }
    
    for(int i = 0; i < pose->boneCount; i++)
    {
        Cyb_Identity(&pose->matrices[i]);
    }

    pose->bones = (Cyb_Mat4*)SDL_malloc(sizeof(Cyb_Mat4) * pose->boneCount);
    
    if(!pose->bones)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "[CybRender] Out of Memory");
        Cyb_FreeObject((Cyb_Object**)&pose);
        return NULL;
    }
    
    return pose;
}


void Cyb_UpdateBone(Cyb_Armature *armature, Cyb_Pose *pose, int bone, 
    const Cyb_Mat4 *matrix)
{
    //Copy the new bone transform
    memcpy(&pose->matrices[bone], matrix, sizeof(Cyb_Mat4));
    
    //Update the bone matrix
    Cyb_Mat4 *boneMatrix = &armature->bones[bone].matrix;
    Cyb_Mat4 *poseBoneMatrix = &pose->bones[bone];
    Cyb_Mat4 tmp;
    Cyb_MulMat4(&tmp, boneMatrix, matrix);
    //Cyb_MulMat4(poseBoneMatrix, &tmp, )
}