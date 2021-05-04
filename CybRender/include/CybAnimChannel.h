#ifndef CYBANIMCHANNEL_H
#define CYBANIMCHANNEL_H

/** @file
 * @brief CybRender - Animation Channel API
 */
 
#include "CybArmature.h"
#include "CybCommon.h"
#include "CybKeyframes.h"
#include "CybMath.h"

 
#ifdef __cplusplus
extern "C" {
#endif

//Types
//=================================================================================
typedef struct Cyb_AnimChannel Cyb_AnimChannel;


//Functions
//=================================================================================
CYBAPI Cyb_AnimChannel *Cyb_CreateAnimChannel(void);
CYBAPI void Cyb_UpdateAnimChannel(Cyb_AnimChannel *animChannel, const char *name,
    int posKeyCount, const Cyb_VecKey *posKeys, int rotKeyCount, 
    const Cyb_QuatKey *rotKeys, int sclKeyCount, const Cyb_VecKey *sclKeys);
CYBAPI void Cyb_ApplyAnimChannel(Cyb_AnimChannel *animChannel, Cyb_Pose *pose,
    double time);

#ifdef __cplusplus
}
#endif

#endif