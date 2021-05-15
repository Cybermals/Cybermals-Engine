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

/** @addtogroup CybRender
 * @brief Cybermals Engine - Renderer Subsystem
 * @{
 */

//Types
//=================================================================================
/** @brief Animation channel type.
 */
typedef struct Cyb_AnimChannel Cyb_AnimChannel;


//Functions
//=================================================================================
/** @brief Create a new animation channel.
 *
 * @return Pointer to the animation channel.
 */
CYBAPI Cyb_AnimChannel *Cyb_CreateAnimChannel(void);

/** @brief Update an animation channel.
 *
 * @param animChannel Pointer to the animation channel.
 * @param name The name of the animation channel.
 * @param posKeyCount The number of position keys.
 * @param posKeys The position keys.
 * @param rotKeyCount The number of rotation keys.
 * @param rotKeys The rotation keys.
 * @param sclKeyCount The number of scale keys.
 * @param sclKeys The scale keys.
 */
CYBAPI void Cyb_UpdateAnimChannel(Cyb_AnimChannel *animChannel, const char *name,
    int posKeyCount, const Cyb_VecKey *posKeys, int rotKeyCount, 
    const Cyb_QuatKey *rotKeys, int sclKeyCount, const Cyb_VecKey *sclKeys);
    
/** @brief Apply an animation channel to a pose.
 *
 * @param animChannel Pointer to the animation channel.
 * @param pose Pointer to the pose.
 * @param time The current animation time.
 *
 * @return TRUE if the animation channel has finished playing.
 */
CYBAPI int Cyb_ApplyAnimChannel(Cyb_AnimChannel *animChannel, Cyb_Pose *pose,
    double time);
    
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif