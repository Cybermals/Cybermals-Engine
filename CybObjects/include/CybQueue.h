#ifndef CYBQUEUE_H
#define CYBQUEUE_H

/** @file
 * @brief CybObjects - Queue API
 */

#include "CybVector.h"
#include "CybObject.h"

 
#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup CybObjects
 * @brief Cybermals Engine - Objects API
 * @{
 */

//Structures
//=================================================================================
/** @brief Queue structure and type.
 */
typedef struct
{
    Cyb_Object base;         /**< Base object. (read-only) */
    size_t elmSize;          /**< Element size. (read-only) */
    Cyb_FreeElmProc freeElm; /**< Element destructor. (read-only) */
    void *buf;               /**< Queue ring buffer. (read-only) */
    size_t head;             /**< Queue head pos. (read-only) */
    size_t tail;             /**< Queue tail pos. (read-only) */
    size_t len;              /**< Queue length. (read-only) */
    size_t size;             /**< Maximum queue size. (read-only) */
} Cyb_Queue;


//Functions
//=================================================================================
/** @brief Create a new queue.
 *
 * @param elmSize The size of each queue element.
 * @param destructor Queue element destructor.
 * @param size Maximum queue size (in elements).
 *
 * @return Pointer to the queue.
 */
CYBAPI Cyb_Queue *Cyb_CreateQueue(size_t elmSize, Cyb_FreeElmProc destructor,
    size_t size);
    
/** @brief Put data into the given queue.
 *
 * @param queue Pointer to the queue.
 * @param data Pointer to the data.
 */
CYBAPI void Cyb_Enqueue(Cyb_Queue *queue, const void *data);

/** @brief Threadsafe version of Cyb_Enqueue.
 *
 * @param queue Pointer to the queue.
 * @param data Pointer to the data.
 */
CYBAPI void Cyb_SafeEnqueue(Cyb_Queue *queue, const void *data);

/** @brief Remove data from the given queue.
 *
 * @param queue Pointer to the queue.
 * @param data Pointer to the memory block that will receive the data.
 *
 * @return TRUE if data was fetched from the queue or FALSE if the queue was empty.
 */
CYBAPI int Cyb_Dequeue(Cyb_Queue *queue, void *data);

/** @brief Threadsafe version of Cyb_Dequeue.
 *
 * @param queue Pointer to the queue.
 * @param data Pointer to the memory block that will receive the data.
 *
 * @return TRUE if data was fetched from the queue or FALSE if the queue was empty.
 */
CYBAPI int Cyb_SafeDequeue(Cyb_Queue *queue, void *data);

/** @brief Check if the given queue is empty.
 *
 * @param queue Pointer to the queue.
 *
 * @return TRUE if the queue is empty.
 */
CYBAPI int Cyb_IsQueueEmpty(const Cyb_Queue *queue);

/** @brief Threadsafe version of Cyb_IsQueueEmpty.
 *
 * @param queue Pointer to the queue.
 *
 * @return TRUE if the queue is empty.
 */
CYBAPI int Cyb_SafeIsQueueEmpty(Cyb_Queue *queue);

/** @brief Check if the given queue is full.
 *
 * @param queue Pointer to the queue.
 *
 * @return TRUE if the queue is full.
 */
CYBAPI int Cyb_IsQueueFull(const Cyb_Queue *queue);

/** @brief Threadsafe version of Cyb_IsQueueFull.
 *
 * @param queue Pointer to the queue.
 *
 * @return TRUE if the queue is full.
 */
CYBAPI int Cyb_SafeIsQueueFull(Cyb_Queue *queue);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif