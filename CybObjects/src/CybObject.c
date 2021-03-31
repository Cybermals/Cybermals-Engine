/*
CybObject - Object API
*/

#include <stdlib.h>

#include "CybObject.h"


//Functions
//==================================================================================
Cyb_Object *Cyb_CreateObject(size_t size, Cyb_FreeProc destructor, int type)
{
    //Allocate a new object
    Cyb_Object *obj = (Cyb_Object*)SDL_malloc(size);
    
    if(!obj)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "[CybObject] Out of Memory");
        return NULL;
    }
    
    //Initialize the object
    obj->free = destructor;
    obj->refCnt = 1;
    obj->lock = SDL_CreateMutex();
    obj->type = type;
    
    if(!obj->lock)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[CybObject] %s",
            SDL_GetError());
        free(obj);
        return NULL;
    }
    
    return obj;
}


void Cyb_FreeObject(Cyb_Object **obj)
{
    //Don't free a NULL object
    Cyb_Object *tmp = *obj;
    
    if(!tmp)
    {
        return;
    }
    
    //Update the ref count
    if(--tmp->refCnt == 0)
    {
        //Call the destructor, destroy the mutex, and free the object
        if(tmp->free)
        {
            tmp->free(tmp);
        }
        
        SDL_DestroyMutex(tmp->lock);
        SDL_free(tmp);
    }
    
    //Set the object to NULL
    *obj = NULL;
}


void Cyb_SafeFreeObject(Cyb_Object **obj)
{
    //Lock the object and free it
    Cyb_Object *tmp = *obj;
    Cyb_LockObject(tmp);
    Cyb_FreeObject(obj);
    
    //If the object wasn't disposed of, we need to unlock it
    if(*obj)
    {
        Cyb_UnlockObject(tmp);
    }
}


void Cyb_LockObject(Cyb_Object *obj)
{
    //Lock the given object
    SDL_LockMutex(obj->lock);
}


void Cyb_UnlockObject(Cyb_Object *obj)
{
    //Unlock the given object
    SDL_UnlockMutex(obj->lock);
}


Cyb_Object *Cyb_NewObjectRef(Cyb_Object *obj)
{
    //Update the ref count and return a new ref
    obj->refCnt++;
    return obj;
}


Cyb_Object *Cyb_SafeNewObjectRef(Cyb_Object *obj)
{
    //Lock the object, create a new ref, and unlock the object
    Cyb_LockObject(obj);
    Cyb_Object *ref = Cyb_NewObjectRef(obj);
    Cyb_UnlockObject(obj);
    return ref;
}