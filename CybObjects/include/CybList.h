#ifndef CYBLIST_H
#define CYBLIST_H

/** @file
 * @brief CybObjects - Linked List API
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
/** @brief Special list indices.
 */
enum Cyb_ListIndex
{
    CYB_LIST_START = 0, /**< First list element index. */
    CYB_LIST_END = -1   /**< Last list element index. */
};


//Types
//=================================================================================
/** @brief List node type.
 */
typedef struct Cyb_ListNode Cyb_ListNode;

/** @brief List node freeing procedure.
 *
 * @param node Pointer to the node to free.
 */
typedef void (*Cyb_FreeNodeProc)(Cyb_ListNode *node);


//Structures
//=================================================================================
/** @brief List node structure.
 */
struct Cyb_ListNode
{
    Cyb_ListNode *next; /**< Pointer to the next list node. */
};


/** @brief List structure and type.
 */
typedef struct
{
    Cyb_Object base;           /**< Base object. (read-only) */
    size_t nodeSize;           /**< Size of each list node. (read-only) */
    Cyb_FreeNodeProc freeNode; /**< List node destructor. (read-only) */
    Cyb_ListNode *first;       /**< Pointer to the first list node. (read-only) */
    size_t len;                /**< Current length of the list. (read-only) */
} Cyb_List;


//Functions
//=================================================================================
/** @brief Create a new list.
 *
 * @param nodeSize The size of each node.
 * @param nodeDestructor The destructor for each node.
 *
 * @return Pointer to the new list or NULL.
 */
CYBAPI Cyb_List *Cyb_CreateList(size_t nodeSize, Cyb_FreeNodeProc nodeDestructor);

/** @brief Insert a new element into the given list.
 *
 * @param list Pointer to the list.
 * @param i Index of the element to insert in front of.
 *
 * @return Pointer to the new list element.
 */
CYBAPI Cyb_ListNode *Cyb_InsertListElm(Cyb_List *list, size_t i);

/** @brief Threadsafe version of Cyb_InsertListElm.
 *
 * @param list Pointer to the list.
 * @param i Index of the element to insert in front of.
 *
 * @return Pointer to the new list element.
 */
CYBAPI Cyb_ListNode *Cyb_SafeInsertListElm(Cyb_List *list, size_t i);

/** @brief Remove an element from the given list.
 *
 * @param list Pointer to the list.
 * @param i Index of the element to remove.
 */
CYBAPI void Cyb_RemoveListElm(Cyb_List *list, size_t i);

/** @brief Threadsafe version of Cyb_RemoveListElm.
 *
 * @param list Pointer to the list.
 * @param i Index of the element to remove.
 */
CYBAPI void Cyb_SafeRemoveListElm(Cyb_List *list, size_t i);

/** @brief Get an element in the given list.
 *
 * @param list Pointer to the list.
 * @param i Index of the element to fetch.
 *
 * @return Pointer to the list element.
 */
CYBAPI Cyb_ListNode *Cyb_GetListElm(const Cyb_List *list, size_t i);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif