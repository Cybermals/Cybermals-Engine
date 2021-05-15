/*
CybRender - Animation Channel API
*/

#include "CybAnimChannel.h"
#include "CybObjects.h"


//Structures
//=================================================================================
struct Cyb_AnimChannel
{
    Cyb_Object base;
    char *name;
    int posKeyCount;
    Cyb_VecKey *posKeys;
    int rotKeyCount;
    Cyb_QuatKey *rotKeys;
    int sclKeyCount;
    Cyb_VecKey *sclKeys;
};


//Functions
//=================================================================================
static void Cyb_FreeAnimChannel(Cyb_AnimChannel *animChannel)
{
    //Free name
    if(animChannel->name)
    {
        SDL_free(animChannel->name);
    }
    
    //Free position keys
    if(animChannel->posKeys)
    {
        SDL_free(animChannel->posKeys);
    }
    
    //Free rotation keys
    if(animChannel->rotKeys)
    {
        SDL_free(animChannel->rotKeys);
    }
    
    //Free scale keys
    if(animChannel->sclKeys)
    {
        SDL_free(animChannel->sclKeys);
    }
}


Cyb_AnimChannel *Cyb_CreateAnimChannel(void)
{
    //Allocate new animation channel
    Cyb_AnimChannel *animChannel = (Cyb_AnimChannel*)Cyb_CreateObject(
        sizeof(Cyb_AnimChannel), (Cyb_FreeProc)&Cyb_FreeAnimChannel, 
        CYB_ANIMCHANNEL);
        
    if(!animChannel)
    {
        return NULL;
    }
    
    //Initialize the animation channel
    animChannel->name = NULL;
    animChannel->posKeyCount = 0;
    animChannel->posKeys = NULL;
    animChannel->rotKeyCount = 0;
    animChannel->rotKeys = NULL;
    animChannel->sclKeyCount = 0;
    animChannel->sclKeys = NULL;
    return animChannel;
}


void Cyb_UpdateAnimChannel(Cyb_AnimChannel *animChannel, const char *name,
    int posKeyCount, const Cyb_VecKey *posKeys, int rotKeyCount, 
    const Cyb_QuatKey *rotKeys, int sclKeyCount, const Cyb_VecKey *sclKeys)
{
    //Free old name, position keys, rotation keys, and scale keys
    if(animChannel->name)
    {
        SDL_free(animChannel->name);
    }
    
    if(animChannel->posKeys)
    {
        SDL_free(animChannel->posKeys);
        animChannel->posKeyCount = 0;
    }
    
    if(animChannel->rotKeys)
    {
        SDL_free(animChannel->rotKeys);
        animChannel->rotKeyCount = 0;
    }
    
    if(animChannel->sclKeys)
    {
        SDL_free(animChannel->sclKeys);
        animChannel->sclKeyCount = 0;
    }
    
    //Update animation channel name
    animChannel->name = (char*)SDL_malloc(strlen(name) + 1);
    
    if(!animChannel->name)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s",
            "[CybRender] Out of Memory");
        return;
    }
    
    strcpy(animChannel->name, name);
    
    //Update position keys
    animChannel->posKeyCount = posKeyCount;
    animChannel->posKeys = (Cyb_VecKey*)SDL_malloc(
        sizeof(Cyb_VecKey) * posKeyCount);
        
    if(!animChannel->posKeys)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s",
            "[CybRender] Out of Memory");
        animChannel->posKeyCount = 0;
        return;
    }
    
    memcpy(animChannel->posKeys, posKeys, sizeof(Cyb_VecKey) * posKeyCount);
    
    //Update rotation keys
    animChannel->rotKeyCount = rotKeyCount;
    animChannel->rotKeys = (Cyb_QuatKey*)SDL_malloc(
        sizeof(Cyb_QuatKey) * rotKeyCount);
        
    if(!animChannel->rotKeys)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s",
            "[CybRender] Out of Memory");
        animChannel->rotKeyCount = 0;
        return;
    }
    
    memcpy(animChannel->rotKeys, rotKeys, sizeof(Cyb_QuatKey) * rotKeyCount);
    
    //Update scale keys
    animChannel->sclKeyCount = sclKeyCount;
    animChannel->sclKeys = (Cyb_VecKey*)SDL_malloc(
        sizeof(Cyb_VecKey) * sclKeyCount);
        
    if(!animChannel->sclKeys)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s",
            "[CybRender] Out of Memory");
        animChannel->sclKeyCount = 0;
        return;
    }
    
    memcpy(animChannel->sclKeys, sclKeys, sizeof(Cyb_VecKey) * sclKeyCount);
}


int Cyb_ApplyAnimChannel(Cyb_AnimChannel *animChannel, Cyb_Pose *pose,
    double time)
{
    //Get the ID of the bone to update
    int boneID = Cyb_GetBoneID(pose, animChannel->name);
    
    if(boneID == -1)
    {
        return FALSE;
    }
    
    //Calculate the current position
    int animOver = TRUE;
    Cyb_Vec3 pos = {0.0f, 0.0f, 0.0f};
    
    for(int i = 0; i < animChannel->posKeyCount - 1; i++)
    {
        //Is the current time between this key and the next?
        Cyb_VecKey *a = &animChannel->posKeys[i];
        Cyb_VecKey *b = &animChannel->posKeys[i + 1];
        
        if(time >= a->time && time <= b->time)
        {
            //Calculate progression between keys and interpolate
            double progress =  (time - a->time) / (b->time - a->time);
            Cyb_Lerp(&pos, &a->value, &b->value, progress);
            animOver = FALSE;
        }
    }
    
    //Calculate the current rotation
    Cyb_Vec4 rot = {0.0f, 0.0f, 0.0f, 1.0f};
    
    for(int i = 0; i < animChannel->rotKeyCount - 1; i++)
    {
        //Is the current time between this key and the next?
        Cyb_QuatKey *a = &animChannel->rotKeys[i];
        Cyb_QuatKey *b = &animChannel->rotKeys[i + 1];
        
        if(time >= a->time && time <= b->time)
        {
            //Calculate progression between keys and interpolate
            double progress =  (time - a->time) / (b->time - a->time);
            Cyb_Slerp(&rot, &a->value, &b->value, progress);
            animOver = FALSE;
        }
    }
    
    //Calculate the current scale
    Cyb_Vec3 scale = {1.0f, 1.0f, 1.0f};
    
    for(int i = 0; i < animChannel->sclKeyCount - 1; i++)
    {
        //Is the current time between this key and the next?
        Cyb_VecKey *a = &animChannel->sclKeys[i];
        Cyb_VecKey *b = &animChannel->sclKeys[i + 1];
        
        if(time >= a->time && time <= b->time)
        {
            //Calculate progression between keys and interpolate
            double progress =  (time - a->time) / (b->time - a->time);
            Cyb_Lerp(&scale, &a->value, &b->value, progress);
            animOver = FALSE;
        }
    }
    
    //Build transformation matrix
    Cyb_Mat4 s;
    Cyb_Mat4 r;
    Cyb_Mat4 t;
    Cyb_Mat4 tmp;
    Cyb_Mat4 m;
    Cyb_Scale(&s, scale.x, scale.y, scale.z);
    Cyb_QuatToMatrix(&r, &rot);
    Cyb_Translate(&t, pos.x, pos.y, pos.z);
    Cyb_MulMat4(&tmp, &t, &r);
    Cyb_MulMat4(&m, &tmp, &s);
    
    //Update the bone that is controlled by this channel
    Cyb_UpdateBone(pose, boneID, &m);
    return animOver;
}