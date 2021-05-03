#ifndef CYBANIMATION_H
#define CYBANIMATION_H

/** @file
 * @brief CybRender - Animation API
 */
 
#include "CybAnimChannel.h"
#include "CybCommon.h"

 
#ifdef __cplusplus
extern "C" {
#endif

//Types
//=================================================================================
typedef struct Cyb_Animation Cyb_Animation;


//Functions
//=================================================================================
CYBAPI Cyb_Animation *Cyb_CreateAnimation(double ticksPerSecond);
CYBAPI Cyb_AnimChannel *Cyb_AddAnimChannel(Cyb_Animation *anim);
CYBAPI void Cyb_DelAnimChannel(Cyb_Animation *anim, int channel);
CYBAPI Cyb_AnimChannel *Cyb_GetAnimChannel(Cyb_Animation *anim, int channel);

#ifdef __cplusplus
}
#endif

#endif