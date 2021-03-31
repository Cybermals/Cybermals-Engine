/*
CybObjects - Linked List API
*/

#include "CybList.h"


//Functions
//==================================================================================
void Cyb_FreeList(Cyb_List *list)
{
    //Iterate over all the nodes and free each one
    Cyb_ListNode *node = list->first;
    
    while(node)
    {
        Cyb_ListNode *next = node->next;
        
        if(list->freeNode)
        {
            list->freeNode(node);
        }
        
        SDL_free(node);
        node = next;
    }
}


Cyb_List *Cyb_CreateList(size_t nodeSize, Cyb_FreeNodeProc nodeDestructor)
{
    //Allocate new list
    Cyb_List *list = (Cyb_List*)Cyb_CreateObject(sizeof(Cyb_List), 
        (Cyb_FreeProc)&Cyb_FreeList, CYB_LIST);
        
    if(!list)
    {
        return NULL;
    }
    
    //Initialize the list
    list->nodeSize = nodeSize;
    list->freeNode = nodeDestructor;
    list->first = NULL;
    list->len = 0;
    return list;
}


Cyb_ListNode *Cyb_InsertListElm(Cyb_List *list, size_t i)
{
    //Allocate new list node
    Cyb_ListNode *node = (Cyb_ListNode*)SDL_malloc(list->nodeSize);
    
    if(!node)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "[CybObjects] Out of Memory");
        return NULL;
    }
    
    //Are we inserting at the beginning?
    if(i == CYB_LIST_START)
    {
        //Replace the first node with the new node and make the first node a child
        //of the second node.
        Cyb_ListNode *node2 = list->first;
        list->first = node;
        node->next = node2;
        
        //Update list length
        list->len++;
    }
    //Are we inserting at the end?
    else if(i == CYB_LIST_END || i == list->len)
    {
        //Is the first node the last?
        if(!list->first)
        {
            //Replace the first node with the new node and make the first node a 
            //child of the second node.
            Cyb_ListNode *node2 = list->first;
            list->first = node;
            node->next = node2;
            
            //Update list length
            list->len++;
            return node;
        }
        
        //Find the last node
        Cyb_ListNode *last = list->first;
        
        while(last)
        {
            //Is this the last node?
            if(!last->next)
            {
                break;
            }
            
            last = last->next;
        }
        
        //Make the new node a child of the last node and set the child of the new
        //node to NULL.
        last->next = node;
        node->next = NULL;
        
        //Update list length
        list->len++;
    }
    //Are we inserting in the middle?
    else
    {
        //Negative values are relative to the end of the list
        int n = i;
        
        if(n < 0)
        {
            n += list->len;
        }
        
        //Ensure that the index is within the list
        if(n < 0 || n > list->len - 1)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", 
                "[CybObjects] List insert index out of bounds.");
            SDL_free(node);
            return NULL;
        }
    
        //Find the nodes before and after the insert position
        Cyb_ListNode *before = list->first;
        Cyb_ListNode *after = before->next;
        
        for(size_t pos = 0; pos < n - 1; pos++)
        {
            before = after;
            after = before->next;
        }
        
        //Insert the new node between the 2 nodes
        before->next = node;
        node->next = after;
        list->len++;
    }
    
    //Return new node
    return node;
}


Cyb_ListNode *Cyb_SafeInsertListElm(Cyb_List *list, size_t i)
{
    //Lock the list, insert the element, and unlock the list
    Cyb_LockObject((Cyb_Object*)list);
    Cyb_ListNode *node = Cyb_InsertListElm(list, i);
    Cyb_UnlockObject((Cyb_Object*)list);
    return node;
}


void Cyb_RemoveListElm(Cyb_List *list, size_t i)
{
    //Can't remove from an empty list!
    if(list->len == 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "[CybObjects] Cannot remove an element from an empty list!");
        return;
    }
    
    //Remove first node?
    if(i == CYB_LIST_START)
    {
        //Make the second node the new first node and free the old first node.
        Cyb_ListNode *node = list->first;
        list->first = node->next;
        list->freeNode(node);
        SDL_free(node);
        
        //Update list length
        list->len--;
    }
    //Remove last node?
    else if(i == CYB_LIST_END || i == list->len - 1)
    {
        //Find the last node and its parent
        Cyb_ListNode *node = list->first;
        Cyb_ListNode *parent = NULL;
        
        while(node->next)
        {
            parent = node;
            node = node->next;
        }
        
        //Remove the node from its parent and free it
        if(parent)
        {
            parent->next = NULL;
        }
        else
        {
            list->first = NULL;
        }
        
        list->freeNode(node);
        SDL_free(node);
        
        //Update list length
        list->len--;
    }
    //Remove a node in the middle?
    else
    {
        //Negative values are relative to the end of the list
        int n = i;
        
        if(n < 0)
        {
            n += list->len;
        }
        
        //Ensure that the index is within the list
        if(n < 0 || n > list->len - 2)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", 
                "[CybObjects] List remove index out of bounds.");
            return;
        }
        
        //Find the node to remove and its parent
        Cyb_ListNode *parent = list->first;
        Cyb_ListNode *node = parent->next;
        
        for(size_t pos = 0; pos < n - 1; pos++)
        {
            parent = node;
            node = parent->next;
        }
        
        //Set the new child of the parent to the grandchild and free the node
        parent->next = node->next;
        list->freeNode(node);
        SDL_free(node);
        
        //Update list length
        list->len--;
    }
}


void Cyb_SafeRemoveListElm(Cyb_List *list, size_t i)
{
    //Lock the list, remove the element, and unlock it
    Cyb_LockObject((Cyb_Object*)list);
    Cyb_RemoveListElm(list, i);
    Cyb_UnlockObject((Cyb_Object*)list);
}


Cyb_ListNode *Cyb_GetListElm(const Cyb_List *list, size_t i)
{
    //Negative values are relative to the end of the list
    int n = i;
    
    if(n < 0)
    {
        n += list->len;
    }
        
    //Ensure that the index is within the list
    if(n < 0 || n > list->len - 1)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "[CybObjects] List index out of bounds.");
        return NULL;
    }
    
    //Get the requested node
    Cyb_ListNode *node = list->first;
    
    for(size_t pos = 0; pos < n; pos++)
    {
        node = node->next;
    }
    
    return node;
}