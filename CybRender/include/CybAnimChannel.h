#ifndef CYBANIMCHANNEL_H
#define CYBANIMCHANNEL_H

/** @file
 * @brief CybRender - Animation Channel API
 */
 
#include "CybCommon.h"
#include "CybMath.h"

 
#ifdef __cplusplus
extern "C" {
#endif

//Types
//=================================================================================
typedef struct Cyb_AnimChannel Cyb_AnimChannel;


//Structures
//=================================================================================
typedef struct
{
    double time;
    Cyb_Vec3 value;
} Cyb_VecKey;


typedef struct
{
    double time;
    Cyb_Vec4 value;
} Cyb_QuatKey;


//Functions
//=================================================================================
CYBAPI Cyb_AnimChannel *Cyb_CreateAnimChannel(void);
CYBAPI void Cyb_UpdateAnimChannel(Cyb_AnimChannel *animChannel, const char *name,
    int posKeyCount, const Cyb_VecKey *posKeys, int rotKeyCount, 
    const Cyb_QuatKey *quatKeys, int sclKeyCount, const Cyb_VecKey *sclKeys);

#ifdef __cplusplus
}
#endif

#endif