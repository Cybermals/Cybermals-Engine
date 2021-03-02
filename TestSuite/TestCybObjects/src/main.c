/*
CybObjects - Test Program
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CybObjects.h>


//Globals
//===========================================================================
int wasObjFreed = FALSE;


//Functions
//===========================================================================
void TestFini(void)
{
    puts("Object freed.");
    wasObjFreed = TRUE;
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
        puts("CybObjects test failed.");
        return 1;
    }
    
    puts("CybObjects test succeeded.");
    return 0;
}