/*
CybRender - Animation API
*/

#include "CybAnimation.h"
#include "CybObjects.h"


//Structures
//=================================================================================
struct Cyb_Animation
{
    Cyb_Object base;
    double ticksPerSecond;
    Cyb_List *channels;
};


typedef struct
{
    Cyb_ListNode base;
    Cyb_AnimChannel *channel;
} Cyb_AnimChannelNode;


//Functions
//=================================================================================
static void Cyb_FreeAnimation(Cyb_Animation *anim)
{
    //Free animation channels
    if(anim->channels)
    {
        Cyb_FreeObject((Cyb_Object**)&anim->channels);
    }
}


static void Cyb_FreeAnimChannelNode(Cyb_AnimChannelNode *node)
{
    //Free animation channel
    if(node->channel)
    {
        Cyb_FreeObject((Cyb_Object**)&node->channel);
    }
}


Cyb_Animation *Cyb_CreateAnimation(double ticksPerSecond)
{
    //Allocate new animation
    Cyb_Animation *anim = (Cyb_Animation*)Cyb_CreateObject(sizeof(Cyb_Animation),
        (Cyb_FreeProc)&Cyb_FreeAnimation, CYB_ANIMATION);
        
    if(!anim)
    {
        return NULL;
    }
    
    //Initialize the animation
    anim->ticksPerSecond = ticksPerSecond;
    anim->channels = Cyb_CreateList(sizeof(Cyb_AnimChannelNode),
        (Cyb_FreeNodeProc)&Cyb_FreeAnimChannelNode);
        
    if(!anim->channels)
    {
        Cyb_FreeObject((Cyb_Object**)&anim->channels);
        return NULL;
    }
    
    return anim;
}


Cyb_AnimChannel *Cyb_AddAnimChannel(Cyb_Animation *anim)
{
    //Insert new animation channel
    Cyb_AnimChannelNode *node = (Cyb_AnimChannelNode*)Cyb_InsertListElm(
        anim->channels, CYB_LIST_END);
        
    if(!node)
    {
        return NULL;
    }
    
    node->channel = Cyb_CreateAnimChannel();
    
    if(!node->channel)
    {
        Cyb_RemoveListElm(anim->channels, CYB_LIST_END);
        return NULL;
    }
    
    return node->channel;
}


void Cyb_DelAnimChannel(Cyb_Animation *anim, int channel)
{
    //Remove animation channel
    Cyb_RemoveListElm(anim->channels, channel);
}


Cyb_AnimChannel *Cyb_GetAnimChannel(Cyb_Animation *anim, int channel)
{
    Cyb_AnimChannelNode *node = (Cyb_AnimChannelNode*)Cyb_GetListElm(
        anim->channels, channel);
    
    if(!node)
    {
        return NULL;
    }
    
    return node->channel;
}


int Cyb_ApplyAnimation(Cyb_Animation *anim, Cyb_Pose *pose, double time)
{
    //Apply each animation channel
    int animOver = FALSE;
    
    for(Cyb_AnimChannelNode *node = (Cyb_AnimChannelNode*)anim->channels->first;
        node; node = (Cyb_AnimChannelNode*)node->base.next)
    {
        animOver |= Cyb_ApplyAnimChannel(node->channel, pose, 
            time * anim->ticksPerSecond);
    }
    
    return animOver;
}