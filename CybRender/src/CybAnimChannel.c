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
    const Cyb_QuatKey *quatKeys, int sclKeyCount, const Cyb_VecKey *sclKeys)
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
}