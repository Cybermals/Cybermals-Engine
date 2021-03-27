#ifndef CYBVECTOR_H
#define CYBVECTOR_H

/** @file
 * @brief CybObjects - Vector API
 */
 
#include "CybObject.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup CybObjects
 * @brief Cybermals Engine - Objects API
 * @{
 */

//Enums
//=================================================================================
/** @brief Special vector indices.
 */
enum Cyb_VecIndex
{
    CYB_VEC_START = 0, /**< First vector element index. */
    CYB_VEC_END = -1   /**< Last vector element index. */
};


//Types
//=================================================================================
/** @brief Element freeing procedure.
 *
 * @param elm Pointer to the element to free.
 */
typedef void (*CybFreeElmProc)(void *elm);


//Structures
//=================================================================================
/** @brief Vector structure and type.
 */
typedef struct
{
    Cyb_Object base;        /**< Base object. (read-only) */
    size_t elmSize;         /**< Size of each element. (read-only) */
    CybFreeElmProc freeElm; /**< Element destructor. (read-only) */
    void *data;             /**< Pointer to the internal array. (read-only) */
    size_t size;            /**< Maximum vector length in elements. (read-only) */
    size_t len;             /**< Current vector length. (read-only) */
} Cyb_Vector;


//Functions
//=================================================================================
/** @brief Create a new vector.
 *
 * @param elmSize The size of each element.
 * @param destructor The element destructor.
 *
 * @return Pointer to the vector.
 */
CYBAPI Cyb_Vector *Cyb_CreateVec(size_t elmSize, CybFreeElmProc destructor);

/** @brief Insert a new element into the given vector.
 *
 * @param vec Pointer to the vector.
 * @param i The index of the element to insert before.
 *
 * @return Pointer to the new element.
 */
CYBAPI void *Cyb_InsertVecElm(Cyb_Vector *vec, size_t i);

/** @brief Threadsafe version of Cyb_InsertVecElm.
 *
 * @param vec Pointer to the vector.
 * @param i The index of the element to insert before.
 *
 * @return Pointer to the new element.
 */
CYBAPI void *Cyb_SafeInsertVecElm(Cyb_Vector *vec, size_t i);

/** @brief Remove an element from the given vector.
 *
 * @param vec Pointer to the vector.
 * @param i The index of the element to remove.
 */
CYBAPI void Cyb_RemoveVecElm(Cyb_Vector *vec, size_t i);

/** @brief Threadsafe version of Cyb_RemoveVecElm.
 *
 * @param vec Pointer to the vector.
 * @param i The index of the element to remove.
 */
CYBAPI void Cyb_SafeRemoveVecElm(Cyb_Vector *vec, size_t i);

/** @brief Get an element from a given vector.
 *
 * @param vec Pointer to the vector.
 * @param i The index of the element to fetch.
 *
 * @return Pointer to the requested element.
 */
CYBAPI void *Cyb_GetVecElm(Cyb_Vector *vec, size_t i);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif