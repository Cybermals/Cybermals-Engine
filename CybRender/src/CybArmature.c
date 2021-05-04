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


struct Cyb_Pose
{
    Cyb_Object base;
    Cyb_Armature *armature;
    int boneCount;
    Cyb_Mat4 *matrices;
    Cyb_Mat4 *bones;
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
    pose->armature = armature;
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
    
    for(int i = 0; i < pose->boneCount; i++)
    {
        Cyb_Identity(&pose->bones[i]);
    }
    
    return pose;
}


int Cyb_GetBoneID(Cyb_Pose *pose, const char *name)
{
    //Find the requested bone
    Cyb_Armature *armature = pose->armature;
    
    for(int i = 0; i < armature->boneCount; i++)
    {
        //Is this the bone?
        if(strcmp(armature->bones[i].name, name) == 0)
        {
            return i;
        }
    }
    
    return -1;
}


void Cyb_UpdateBone(Cyb_Pose *pose, int boneID, const Cyb_Mat4 *matrix)
{
    //Fetch bone data
    Cyb_Bone *bone = &pose->armature->bones[boneID];
    Cyb_Mat4 *boneMatrix = &bone->matrix;
    Cyb_Mat4 *poseMatrix = &pose->matrices[boneID];
    Cyb_Mat4 *poseBoneMatrix = &pose->bones[boneID];
    Cyb_Mat4 invBoneMatrix;
    Cyb_Invert(&invBoneMatrix, &bone->matrix);
    Cyb_Mat4 *parentMatrix = NULL;
    
    if(bone->parent > -1)
    {
        parentMatrix = &pose->bones[bone->parent];
    }
    
    //Update pose matrix
    if(matrix)
    {
        memcpy(poseMatrix, matrix, sizeof(Cyb_Mat4));
    }
    
    //Update bone matrix
    Cyb_Armature *armature = pose->armature;
    
    if(parentMatrix)
    {
        Cyb_Mat4 tmp;
        Cyb_Mat4 tmp2;
        Cyb_MulMat4(&tmp, &invBoneMatrix, poseMatrix);
        Cyb_MulMat4(&tmp2, &tmp, boneMatrix);
        Cyb_MulMat4(poseBoneMatrix, parentMatrix, &tmp2);
    }
    else
    {
        Cyb_Mat4 tmp;
        Cyb_MulMat4(&tmp, &invBoneMatrix, poseMatrix);
        Cyb_MulMat4(poseBoneMatrix, &tmp, boneMatrix);
    }
    
    //Update child bones
    for(int i = 0; i < pose->boneCount; i++)
    {
        //Is this bone a child of the bone we updated?
        Cyb_Bone *child = &pose->armature->bones[i];
        
        if(child->parent == boneID)
        {
            Cyb_UpdateBone(pose, i, NULL);
        }
    }
}


void Cyb_SelectPose(Cyb_Renderer *renderer, Cyb_Shader *shader, Cyb_Pose *pose)
{
    //Select renderer
    Cyb_Armature *armature = pose->armature;
    Cyb_SelectRenderer(renderer);
    Cyb_GLExtAPI *glExtAPI = Cyb_GetGLExtAPI(renderer);
    
    //Bind armature VBO
    glExtAPI->BindBuffer(GL_ARRAY_BUFFER, armature->vbo);
    
    //Setup vertex attrib pointers
    glExtAPI->EnableVertexAttribArray(4);
    glExtAPI->EnableVertexAttribArray(5);
    glExtAPI->VertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Cyb_VertexGW),
        (void*)offsetof(Cyb_VertexGW, group));
    glExtAPI->VertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Cyb_VertexGW),
        (void*)offsetof(Cyb_VertexGW, weight));
        
    //Setup bone matrices
    Cyb_SetMatrices(renderer, shader, "bones", pose->boneCount, pose->bones);
}