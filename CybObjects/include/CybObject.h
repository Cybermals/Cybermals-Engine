#ifndef CYBOBJECT_H
#define CYBOBJECT_H

/** @file
 * @brief CybObjects - Object API
 */
 
#include <SDL2/SDL.h>
 
#include "CybCommon.h"

 
#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup CybObjects
 * @brief Cybermals Engine - Objects API
 * @{
 */

//Enums
//==================================================================================
enum Cyb_ObjectTypes
{
    //Base objects
    CYB_OBJECT,      /**< Generic object. */
    CYB_LIST,        /**< Linked list object. */
    CYB_VECTOR,      /**< Vector object. */
    CYB_QUEUE,       /**< Queue object. */
    
    //UI objects
    CYB_GRID,        /**< Grid object. */
    CYB_LABEL,       /**< Label object. */
    CYB_BUTTON,      /**< Button object. */
    CYB_TEXTBOX,     /**< Textbox object. */
    CYB_LISTBOX,     /**< Listbox object. */
    CYB_PROGRESSBAR, /**< Progress bar object. */
    
    //Renderer objects
    CYB_RENDERER,    /**< Renderer object. */
    CYB_SHADER,      /**< Shader object. */
    CYB_MESH,        /**< Mesh object. */
    CYB_CAMERA,      /**< Camera object. */
    CYB_TEXTURE,     /**< Texture object. */
    CYB_LIGHT,       /**< Light object. */
    CYB_MATERIAL,    /**< Material object. */
    CYB_ARMATURE,    /**< Armature object. */
    CYB_POSE,        /**< Pose object. */
    CYB_ANIMCHANNEL, /**< Animation channel object. */
    CYB_ANIMATION    /**< Animation object. */
};


//Types
//==================================================================================
/** @brief Base object type.
 */
typedef struct Cyb_Object Cyb_Object;

/** @brief Object freeing procedure.
 *
 * @param obj Pointer to the object to free.
 */
typedef void (*Cyb_FreeProc)(Cyb_Object *obj);


//Structures
//==================================================================================
/** @brief Base object structure.
 */
struct Cyb_Object
{
    Cyb_FreeProc free; /**< The function to call before freeing the object. (read-only) */
    int refCnt;        /**< The object reference count. (read-only) */
    SDL_mutex *lock;   /**< The object mutex. (read-only) */
    int type;          /**< The object type. (read-only) */
};


//Functions
//==================================================================================
/** @brief Create an object.
 *
 * @param size The size of the new object.
 * @param destructor The function that should be called before freeing the object.
 * @param type The type code for the new object.
 *
 * @return Pointer to the new object or NULL.
 */
CYBAPI Cyb_Object *Cyb_CreateObject(size_t size, Cyb_FreeProc destructor, int type);

/** @brief Free a reference to an object.
 *
 * @param obj Pointer to a pointer to the object.
 */
CYBAPI void Cyb_FreeObject(Cyb_Object **obj);

/** @brief Threadsafe version of Cyb_FreeObject.
 *
 * @param obj Pointer to a pointer to the object.
 */
CYBAPI void Cyb_SafeFreeObject(Cyb_Object **obj);

/** @brief Lock a given object.
 *
 * @param obj Pointer to the object.
 */
CYBAPI void Cyb_LockObject(Cyb_Object *obj);

/** @brief Unlock a given object.
 *
 * @param obj Pointer to the object.
 */
CYBAPI void Cyb_UnlockObject(Cyb_Object *obj);

/** @brief Create a new reference to the given object.
 *
 * @param obj Pointer to the object.
 *
 * @return Pointer to the object.
 */
CYBAPI Cyb_Object *Cyb_NewObjectRef(Cyb_Object *obj);

/** @brief Threadsafe version of Cyb_NewObjectRef.
 *
 * @param obj Pointer to the object.
 *
 * @return Pointer to the object.
 */
CYBAPI Cyb_Object *Cyb_SafeNewObjectRef(Cyb_Object *obj);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif