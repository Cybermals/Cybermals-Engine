#ifndef CYBVECTOR_H
#define CYBVECTOR_H

/** @file
 * @brief CybObjects - Vector API
 */
 
#include "CybObject.h"

#ifdef __cplusplus
extern "C" {
#endif

//Enums
//=================================================================================
enum Cyb_VecIndex
{
    CYB_VEC_START,
    CYB_VEC_END
};


//Types
//=================================================================================
typedef void (*CybFreeElmProc)(void *elm);


//Structures
//=================================================================================
typedef struct
{
    Cyb_Object base;
    size_t elmSize;
    CybFreeElmProc freeElm;
    void *data;
    size_t size;
    size_t len;
} Cyb_Vector;


//Functions
//=================================================================================
CYBAPI Cyb_Vector *Cyb_CreateVec(size_t elmSize, CybFreeElmProc destructor);
CYBAPI void *Cyb_InsertVecElm(Cyb_Vector *vec, size_t i);
CYBAPI void *Cyb_SafeInsertVecElm(Cyb_Vector *vec, size_t i);
CYBAPI void Cyb_RemoveVecElm(Cyb_Vector *vec, size_t i);
CYBAPI void Cyb_SafeRemoveVecElm(Cyb_Vector *vec, size_t i);
CYBAPI void *Cyb_GetVecElm(Cyb_Vector *vec, size_t i);

#ifdef __cplusplus
}
#endif

#endif