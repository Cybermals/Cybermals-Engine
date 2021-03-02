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

//Enums
//==================================================================================
enum
{
    CYB_OBJECT
};


//Types
//==================================================================================
typedef void (*CybFreeProc)(void);


//Structures
//==================================================================================
typedef struct
{
    CybFreeProc free;
    int refCnt;
    SDL_mutex *lock;
    int type;
} Cyb_Object;


//Functions
//==================================================================================
CYBAPI Cyb_Object *Cyb_CreateObject(size_t size, CybFreeProc destructor, int type);
CYBAPI void Cyb_FreeObject(Cyb_Object **obj);
CYBAPI void Cyb_SafeFreeObject(Cyb_Object **obj);
CYBAPI void Cyb_LockObject(Cyb_Object *obj);
CYBAPI void Cyb_UnlockObject(Cyb_Object *obj);
CYBAPI Cyb_Object *Cyb_NewObjectRef(Cyb_Object *obj);
CYBAPI int Cyb_GetObjectType(Cyb_Object *obj);

#ifdef __cplusplus
}
#endif

#endif