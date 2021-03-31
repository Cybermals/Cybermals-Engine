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
        char *elm = (char*)vec->data;
    
        for(size_t i = 0; i < vec->len; i++)
        {
            vec->freeElm(elm);
            elm += vec->elmSize;
        }
    }
    
    //Free the internal array
    SDL_free(vec->data);
}


Cyb_Vector *Cyb_CreateVec(size_t elmSize, Cyb_FreeElmProc destructor)
{
    //Allocate new vector
    Cyb_Vector *vec = (Cyb_Vector*)Cyb_CreateObject(sizeof(Cyb_Vector),
        (Cyb_FreeProc)&Cyb_FreeVec, CYB_VECTOR);
        
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
        memmove((char*)vec->data + vec->elmSize, vec->data, vec->len * vec->elmSize);
        vec->len++;
        return vec->data;
    }
    //Insert at end?
    else if(i == CYB_VEC_END || i == vec->len)
    {
        //No moving required, just return a pointer to the last element
        vec->len++;
        return (char*)vec->data + vec->elmSize * (vec->len - 1);
    }
    //Insert in the middle?
    else
    {
        //Handle negative index
        int n = i;
        
        if(n < 0)
        {
            n += vec->len;
        }
        
        //Do bounds check
        if(n < 0 || n > vec->len - 1)
        {
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s", 
                "[CybObjects] Vector insert index out of bounds. Defaulting to end of vector.");
            vec->len++;
            return (char*)vec->data + vec->elmSize * (vec->len - 1);
        }
        
        //Move the data after the insert position over one element
        memmove((char*)vec->data + vec->elmSize * (n + 1), 
            (char*)vec->data + vec->elmSize * n, vec->elmSize * (vec->len - n));
        vec->len++;
        return (char*)vec->data + vec->elmSize * n;
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
    //Can't remove from an empty vector!
    if(vec->len == 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "[CybObjects] Cannot remove an element from an empty vector!");
        return;
    }
    
    //Remove from beginning?
    if(i == CYB_VEC_START)
    {
        //Move the entire array over by the size of one element
        vec->len--;
        memmove(vec->data, (char*)vec->data + vec->elmSize, vec->len * vec->elmSize);
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
        int n = i;
        
        if(n < 0)
        {
            n += vec->len;
        }
        
        //Do bounds check
        if(n < 0 || n > vec->len - 1)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", 
                "[CybObjects] Vector remove index out of bounds.");
            return;
        }
        
        //Move the data after the remove position over one element
        vec->len--;
        memmove((char*)vec->data + vec->elmSize * n, 
            (char*)vec->data + vec->elmSize * (n + 1), 
            vec->elmSize * (vec->len - n));
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
    int n = i;
    
    if(n < 0)
    {
        n += vec->len;
    }
        
    //Do bounds check
    if(n < 0 || n > vec->len - 1)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "[CybObjects] Vector index out of bounds.");
        return NULL;
    }
    
    //Return a pointer to the requested element
    return (char*)vec->data + vec->elmSize * n;
}