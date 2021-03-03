/*
CybObjects - Vector API
*/

#include <string.h>

#include "CybVector.h"


//Functions
//=================================================================================
void Cyb_FreeVec(Cyb_Vector *vec)
{
    //Call element destructor for each element
    if(vec->freeElm)
    {
        void *elm = vec->data;
    
        for(size_t i = 0; i < vec->len; i++)
        {
            vec->freeElm(elm);
            elm += vec->elmSize;
        }
    }
    
    //Free the internal array
    SDL_free(vec->data);
}


Cyb_Vector *Cyb_CreateVec(size_t elmSize, CybFreeElmProc destructor)
{
    //Allocate new vector
    Cyb_Vector *vec = (Cyb_Vector*)Cyb_CreateObject(sizeof(Cyb_Vector),
        (CybFreeProc)&Cyb_FreeVec, CYB_VECTOR);
        
    if(!vec)
    {
        return NULL;
    }
    
    //Initialize the vector
    vec->elmSize = elmSize;
    vec->freeElm = destructor;
    vec->data = SDL_malloc(elmSize * 4);
    vec->size = 4;
    vec->len = 0;
    
    if(!vec->data)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "[CybObjects] Out of Memory");
        Cyb_FreeObject((Cyb_Object**)&vec);
        return NULL;
    }
    
    return vec;
}


void *Cyb_InsertVecElm(Cyb_Vector *vec, size_t i)
{
    //Resize the internal array as needed
    if(vec->len == vec->size)
    {
        //Double the size of the array
        void *newData = SDL_realloc(vec->data, vec->elmSize * vec->size * 2);
        
        if(!newData)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", 
                "[CybObjects] Out of Memory");
            return NULL;
        }
        
        //Store new array pointer and update size
        vec->data = newData;
        vec->size *= 2;
    }
    
    //Insert at beginning?
    if(i == CYB_VEC_START)
    {
        //Move the entire array over by the size of one element
        memmove(vec->data + vec->elmSize, vec->data, vec->len * vec->elmSize);
        vec->len++;
        return vec->data;
    }
    //Insert at end?
    else if(i == CYB_VEC_END || i == vec->len - 1)
    {
        //No moving required, just return a pointer to the last element
        vec->len++;
        return vec->data + vec->elmSize * (vec->len - 1);
    }
    //Insert in the middle?
    else
    {
        //Handle negative index
        if(i < 0)
        {
            i += vec->len;
        }
        
        //Do bounds check
        if(i < 0 || i > vec->len - 1)
        {
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s", 
                "[CybObjects] Vector insert index out of bounds. Defaulting to end of vector.");
            vec->len++;
            return vec->data + vec->elmSize * (vec->len - 1);
        }
        
        //Move the data after the insert position over one element
        memmove(vec->data + vec->elmSize * (i + 1), vec->data + vec->elmSize * i, 
            vec->elmSize * (vec->len - i));
        vec->len++;
        return vec->data + vec->elmSize * i;
    }
}


void *Cyb_SafeInsertVecElm(Cyb_Vector *vec, size_t i)
{
    //Lock the vector, insert a new element, and unlock the vector
    Cyb_LockObject((Cyb_Object*)vec);
    void *elm = Cyb_InsertVecElm(vec, i);
    Cyb_UnlockObject((Cyb_Object*)vec);
    return elm;
}


void Cyb_RemoveVecElm(Cyb_Vector *vec, size_t i)
{
    //Remove from beginning?
    if(i == CYB_VEC_START)
    {
        //Move the entire array over by the size of one element
        vec->len--;
        memmove(vec->data, vec->data + vec->elmSize, vec->len * vec->elmSize);
    }
    //Remove from end?
    else if(i == CYB_VEC_END || i == vec->len - 1)
    {
        //No moving required, just adjust the length
        vec->len--;
    }
    //Remove from the middle?
    else
    {
        //Handle negative index
        if(i < 0)
        {
            i += vec->len;
        }
        
        //Do bounds check
        if(i < 0 || i > vec->len - 1)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", 
                "[CybObjects] Vector remove index out of bounds.");
            return;
        }
        
        //Move the data after the remove position over one element
        vec->len--;
        memmove(vec->data + vec->elmSize * i, vec->data + vec->elmSize * (i + 1), 
            vec->elmSize * (vec->len - i));
    }
}


void Cyb_SafeRemoveVecElm(Cyb_Vector *vec, size_t i)
{
    //Lock the vector, remove an element, and unlock the vector
    Cyb_LockObject((Cyb_Object*)vec);
    Cyb_RemoveVecElm(vec, i);
    Cyb_UnlockObject((Cyb_Object*)vec);
}


void *Cyb_GetVecElm(Cyb_Vector *vec, size_t i)
{
    //Handle negative index
    if(i < 0)
    {
        i += vec->len;
    }
        
    //Do bounds check
    if(i < 0 || i > vec->len - 1)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "[CybObjects] Vector index out of bounds.");
        return NULL;
    }
    
    //Return a pointer to the requested element
    return vec->data + vec->elmSize * i;
}