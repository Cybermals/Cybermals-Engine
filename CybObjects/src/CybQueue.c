/*
CybObjects - Queue API
*/

#include <string.h>

#include "CybQueue.h"


//Functions
//=================================================================================
void Cyb_FreeQueue(Cyb_Queue *queue)
{
    //Free all elements
    int i = 0;
    
    while(queue->len--)
    {   
        //Free next element
        if(queue->freeElm)
        {
            queue->freeElm((char*)queue->buf + queue->elmSize * i);
        }
        
        //Update pos
        if(++i == queue->size)
        {
            i = 0;
        }
    }
    
    //Free the internal buffer
    SDL_free(queue->buf);
}


Cyb_Queue *Cyb_CreateQueue(size_t elmSize, CybFreeElmProc destructor,
    size_t size)
{
    //Allocate a new queue
    Cyb_Queue *queue = (Cyb_Queue*)Cyb_CreateObject(sizeof(Cyb_Queue),
        (CybFreeProc)&Cyb_FreeQueue, CYB_QUEUE);
        
    if(!queue)
    {
        return NULL;
    }
    
    //Initialize the queue
    queue->elmSize = elmSize;
    queue->freeElm = destructor;
    queue->buf = SDL_malloc(elmSize * size);
    queue->head = 0;
    queue->tail = 0;
    queue->len = 0;
    queue->size = size;
    
    if(!queue->buf)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "[CybObjects] Out of Memory");
        Cyb_FreeObject((Cyb_Object**)&queue);
        return NULL;
    }
    
    return queue;
}


void Cyb_Enqueue(Cyb_Queue *queue, const void *data)
{
    //Verify that the queue isn't full
    if(Cyb_IsQueueFull(queue))
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s",
            "[CybObjects] The queue is full!");
        return;
    }
    
    //Copy the given data into the queue and update the queue length
    memcpy((char*)queue->buf + queue->elmSize * queue->tail, data, queue->elmSize);
    queue->len++;
    
    //Update tail pos
    if(++queue->tail == queue->size)
    {
        queue->tail = 0;
    }
}


void Cyb_SafeEnqueue(Cyb_Queue *queue, const void *data)
{
    //Lock the queue, place the data into it, and unlock the queue
    Cyb_LockObject((Cyb_Object*)queue);
    Cyb_Enqueue(queue, data);
    Cyb_UnlockObject((Cyb_Object*)queue);
}


int Cyb_Dequeue(Cyb_Queue *queue, void *data)
{
    //Verify that the queue isn't empty
    if(Cyb_IsQueueEmpty(queue))
    {
        return FALSE;
    }
    
    //Copy the data at the head of the queue into the given memory block and
    //update the queue length
    memcpy(data, (char*)queue->buf + queue->elmSize * queue->head, queue->elmSize);
    queue->len--;
    
    //Update head pos
    if(++queue->head == queue->size)
    {
        queue->head = 0;
    }
    
    return TRUE;
}


int Cyb_SafeDequeue(Cyb_Queue *queue, void *data)
{
    //Lock the queue, remove the next piece of data, and unlock it
    Cyb_LockObject((Cyb_Object*)queue);
    int res = Cyb_Dequeue(queue, data);
    Cyb_UnlockObject((Cyb_Object*)queue);
    return res;
}


int Cyb_IsQueueEmpty(const Cyb_Queue *queue)
{
    return queue->len == 0;
}


int Cyb_SafeIsQueueEmpty(Cyb_Queue *queue)
{
    //Lock the queue, check if is empty, and unlock it
    Cyb_LockObject((Cyb_Object*)queue);
    int res = Cyb_IsQueueEmpty(queue);
    Cyb_UnlockObject((Cyb_Object*)queue);
    return res;
}


int Cyb_IsQueueFull(const Cyb_Queue *queue)
{
    return queue->len == queue->size;
}


int Cyb_SafeIsQueueFull(Cyb_Queue *queue)
{
    //Lock the queue, check if is full, and unlock the queue
    Cyb_LockObject((Cyb_Object*)queue);
    int res = Cyb_IsQueueFull(queue);
    Cyb_UnlockObject((Cyb_Object*)queue);
    return res;
}