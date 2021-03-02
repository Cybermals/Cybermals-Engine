/*
CybObjects - Test Program
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CybObjects.h>


//Structures
//===========================================================================
typedef struct
{
    Cyb_ListNode base;
    int value;
} IntListNode;


//Globals
//===========================================================================
int wasObjFreed = FALSE;


//Functions
//===========================================================================
void TestFini(Cyb_Object *obj)
{
    printf("Object at 0x%x freed.\n", obj);
    wasObjFreed = TRUE;
}


void TestNodeFini(Cyb_ListNode *node)
{
    printf("List node at 0x%x freed.\n", node);
}


int TestCybObject(void)
{
    //Create an object
    puts("Creating an object...");
    Cyb_Object *obj = Cyb_CreateObject(sizeof(Cyb_Object), &TestFini, CYB_OBJECT);
    
    if(!obj)
    {
        puts("Failed to create an object.");
        return 1;
    }
    
    //Create some new object refs
    puts("Creating new object refs...");
    Cyb_Object *obj2;
    
    for(int i = 0; i < 10; i++)
    {
        obj2 = Cyb_NewObjectRef(obj);
    }
    
    //Free all the refs
    for(int i = 0; i < 10; i++)
    {
        Cyb_FreeObject(&obj2);
        
        if(obj2)
        {
            puts("Object ref not set to NULL as expected.");
        }
        
        obj2 = obj;
    }
    
    //Free final ref
    puts("Freeing final object ref (this should cause the object itself to be freed).");
    Cyb_FreeObject(&obj);
    
    if(!wasObjFreed)
    {
        puts("Object not freed as expected. Memory leak detected.");
        return 1;
    }
    
    return 0;
}


int TestCybList(void)
{
    //Create a list
    puts("Creating a list...");
    Cyb_List *list = Cyb_CreateList(sizeof(IntListNode), &TestNodeFini);
    
    if(!list)
    {
        puts("Failed to create a list.");
        return 1;
    }
    
    //Insert 3 elements at the beginning
    puts("Inserting elements at the beginning...");
    
    for(int i = 2; i > -1; i--)
    {
        IntListNode *node = (IntListNode*)Cyb_InsertListElm(list, CYB_LIST_START);
        
        if(!node)
        {
            puts("Failed to insert a list element at the beginning.");
            Cyb_FreeObject((Cyb_Object**)&list);
            return 1;
        }
        
        node->value = i;
    }
    
    //Insert 3 elements at the end
    puts("Inserting elements at the end...");
    
    for(int i = 6; i < 9; i++)
    {
        IntListNode *node = (IntListNode*)Cyb_InsertListElm(list, CYB_LIST_END);
        
        if(!node)
        {
            puts("Failed to insert a list element at the end.");
            Cyb_FreeObject((Cyb_Object**)&list);
            return 1;
        }
        
        node->value = i;
    }
    
    //Insert 3 elements in the middle
    puts("Inserting elements in the middle...");
    
    for(int i = 5; i > 2; i--)
    {
        IntListNode *node = (IntListNode*)Cyb_InsertListElm(list, 3);
        
        if(!node)
        {
            puts("Failed to insert a list element in the middle.");
            Cyb_FreeObject((Cyb_Object**)&list);
            return 1;
        }
        
        node->value = i;
    }
    
    //Verify the list items
    puts("Verifying list items...");
    int i = 0;
    
    for(IntListNode *node = (IntListNode*)list->first; node; 
        node = (IntListNode*)node->base.next)
    {
        //Verify the data
        if(node->value != i++)
        {
            printf("The value of element %i was %i, however it should have been %i.\n",
                i - 1, node->value, i - 1);
        }
    }
    
    //Remove the first, last, and middle items
    puts("Removing a few list items...");
    Cyb_RemoveListElm(list, CYB_LIST_START);
    Cyb_RemoveListElm(list, CYB_LIST_END);
    Cyb_RemoveListElm(list, 3);
    
    //Verify the list items again
    puts("Verifying list items...");
    i = 0;
    int values[] = {1, 2, 3, 5, 6, 7};
    
    for(IntListNode *node = (IntListNode*)list->first; node; 
        node = (IntListNode*)node->base.next)
    {
        //Verify the data
        if(node->value != values[i++])
        {
            printf("The value of element %i was %i, however it should have been %i.\n",
                i - 1, node->value, values[i - 1]);
        }
    }
    
    //Test the "get" function
    puts("Fetching a list item...");
    IntListNode *node = (IntListNode*)Cyb_GetListElm(list, 4);
    
    if(!node || node->value != values[4])
    {
        puts("Failed to properly fetch list item.");
        Cyb_FreeObject((Cyb_Object**)&list);
        return 1;
    }
    
    //Test bounds-checking
    puts("Testing bounds-checking (you should see 3 error messages if this succeeds)...");
    node = (IntListNode*)Cyb_InsertListElm(list, 2000);
    
    if(node)
    {
        puts("Bounds-checking failed.");
        Cyb_FreeObject((Cyb_Object**)&list);
        return 1;
    }
    
    node = (IntListNode*)Cyb_GetListElm(list, 2000);
    
    if(node)
    {
        puts("Bounds-checking failed.");
        Cyb_FreeObject((Cyb_Object**)&list);
        return 1;
    }
    
    Cyb_RemoveListElm(list, 2000);
    
    //Free the list
    puts("Freeing list...");
    Cyb_FreeObject((Cyb_Object**)&list);
    return 0;
}


//Entry Point
//===========================================================================
int main(int argc, char **argv)
{
    //Init CybObjects
    if(Cyb_InitObjects())
    {
        puts("CybObjects test failed.");
        return 1;
    }

    //Run object test
    if(TestCybObject())
    {
        puts("CybObjects object test failed.");
        return 1;
    }
    
    //Run list test
    if(TestCybList())
    {
        puts("CybObjects list test failed.");
        return 1;
    }
    
    puts("CybObjects test succeeded.");
    return 0;
}