#ifndef CYBANIMATION_H
#define CYBANIMATION_H

/** @file
 * @brief CybRender - Animation API
 */
 
#include "CybAnimChannel.h"
#include "CybArmature.h"
#include "CybCommon.h"

 
#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup CybRender
 * @brief Cybermals Engine - Renderer Subsystem
 * @{
 */

//Types
//=================================================================================
/** @brief Animation type.
 */
typedef struct Cyb_Animation Cyb_Animation;


//Functions
//=================================================================================
/** @brief Create a new animation.
 *
 * @param ticksPerSecond The number of ticks per second.
 *
 * @return Pointer to the animation.
 */
CYBAPI Cyb_Animation *Cyb_CreateAnimation(double ticksPerSecond);

/** @brief Add a new channel to an animation.
 *
 * @param anim Pointer to the animation.
 *
 * @return Pointer to the animation channel.
 */
CYBAPI Cyb_AnimChannel *Cyb_AddAnimChannel(Cyb_Animation *anim);

/** @brief Delete an animation channel.
 *
 * @param anim Pointer to the animation.
 * @param channel The index of the channel.
 */
CYBAPI void Cyb_DelAnimChannel(Cyb_Animation *anim, int channel);

/** @brief Get an animation channel.
 *
 * @param anim Pointer to the animation.
 * @param channel The index of the channel.
 *
 * @return Pointer to the animation channel.
 */
CYBAPI Cyb_AnimChannel *Cyb_GetAnimChannel(Cyb_Animation *anim, int channel);

/** @brief Apply an animation to a pose.
 *
 * @param anim Pointer to the animation.
 * @param pose Pointer to the pose.
 * @param time The current animation time.
 *
 * @return TRUE if the animation has finished playing.
 */
CYBAPI int Cyb_ApplyAnimation(Cyb_Animation *anim, Cyb_Pose *pose, double time);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif