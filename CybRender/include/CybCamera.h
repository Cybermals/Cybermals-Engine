#ifndef CYBCAMERA_H
#define CYBCAMERA_H

/** @file
 * @brief CybRender - Camera API
 */
 
#include "CybCommon.h"
#include "CybMath.h"

 
#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup CybRender
 * @brief Cybermals Engine - Camera API
 * @{
 */

//Types
//=================================================================================
/** @brief Camera object.
 */
typedef struct Cyb_Camera Cyb_Camera;


//Functions
//=================================================================================
/** @brief Create a new camera object.
 *
 * @return Pointer to the camera.
 */
CYBAPI Cyb_Camera *Cyb_CreateCamera(void);

/** @brief Set the position of a camera.
 *
 * @param cam Pointer to the camera.
 * @param x The new X-coordinate.
 * @param y The new Y-coordinate.
 * @param z The new Z-coordinate.
 */
CYBAPI void Cyb_SetCameraPos(Cyb_Camera *cam, float x, float y, float z);

/** @brief Get the current camera position.
 *
 * @param cam Pointer to the camera.
 *
 * @return Pointer to the camera position.
 */
CYBAPI const Cyb_Vec3 *Cyb_GetCameraPos(Cyb_Camera *cam);

/** @brief Set the rotation of a camera.
 *
 * @param cam Pointer to the camera.
 * @param x The new X-axis rotation.
 * @param y The new Y-axis rotation.
 * @param z The new Z-axis rotation.
 */
CYBAPI void Cyb_SetCameraRot(Cyb_Camera *cam, float x, float y, float z);

/** @brief Get the current camera rotation.
 *
 * @param cam Pointer to the camera.
 *
 * @return Pointer to the camera rotation.
 */
CYBAPI const Cyb_Vec3 *Cyb_GetCameraRot(Cyb_Camera *cam);

/** @brief Set the camera zoom.
 *
 * @param cam Pointer to the camera.
 * @param zoom The new zoom.
 */
CYBAPI void Cyb_SetCameraZoom(Cyb_Camera *cam, float zoom);

/** @brief Get the current camera zoom.
 *
 * @param cam Pointer to the camera.
 *
 * @return The zoom.
 */
CYBAPI float Cyb_GetCameraZoom(Cyb_Camera *cam);

/** @brief Move a camera based on just its yaw.
 *
 * @param cam Pointer to the camera.
 * @param velocity The current camera velocity.
 */
CYBAPI void Cyb_MoveCamera(Cyb_Camera *cam, float velocity);

/** @brief Rotate a camera.
 *
 * @param cam Pointer to the camera.
 * @param The X-axis angle increment.
 * @param the Y-axis angle increment.
 * @param The Z-axis angle increment.
 */
CYBAPI void Cyb_RotateCamera(Cyb_Camera *cam, float x, float y, float z);

/** @brief Aim a camera so it faces the given direction.
 *
 * @param cam Pointer to the camera.
 * @param pos Pointer to the new camera position.
 * @param right Pointer to the new right vector.
 * @param up Pointer to the new up vector.
 * @param dir Pointer to the new direction vector.
 */
CYBAPI void Cyb_AimCamera(Cyb_Camera *cam, Cyb_Vec3 *pos, Cyb_Vec3 *right,
    Cyb_Vec3 *up, Cyb_Vec3 *dir);
    
/** @brief Get the view matrix of a camera.
 *
 * @param cam Pointer to the camera.
 *
 * @return Pointer to the view matrix.
 */
CYBAPI Cyb_Mat4 *Cyb_GetViewMatrix(Cyb_Camera *cam);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif