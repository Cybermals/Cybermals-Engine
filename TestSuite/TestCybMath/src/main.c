/*
CybMath - Test Program
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CybMath.h"


//Entry Point
//===========================================================================
int TestCybVectors(void)
{
    //Test 2D vector addition
    {
        puts("Testing 2D vector addition...");
        Cyb_Vec2 a = {3, 4};
        Cyb_Vec2 b = {1, 2};
        Cyb_Vec2 c;
        Cyb_AddVec2(&c, &a, &b);
        
        if(c.x != 4 || c.y != 6)
        {
            puts("failed");
            return 1;
        }
    }
    
    //Test 2D vector subtraction
    {
        puts("Testing 2D vector subtraction...");
        Cyb_Vec2 a = {3, 4};
        Cyb_Vec2 b = {1, 2};
        Cyb_Vec2 c;
        Cyb_SubVec2(&c, &a, &b);
        
        if(c.x != 2 || c.y != 2)
        {
            puts("failed");
            return 1;
        }
    }
    
    //Test 2D vector multiplication
    {
        puts("Testing 2D vector multiplication...");
        Cyb_Vec2 a = {3, 4};
        Cyb_Vec2 b = {1, 2};
        Cyb_Vec2 c;
        Cyb_MulVec2(&c, &a, &b);
        
        if(c.x != 3 || c.y != 8)
        {
            puts("failed");
            return 1;
        }
    }
    
    //Test 2D vector division
    {
        puts("Testing 2D vector division...");
        Cyb_Vec2 a = {3, 4};
        Cyb_Vec2 b = {1, 2};
        Cyb_Vec2 c;
        Cyb_DivVec2(&c, &a, &b);
        
        if(c.x != 3 || c.y != 2)
        {
            puts("failed");
            return 1;
        }
    }
    
    //Test 3D vector addition
    {
        puts("Testing 3D vector addition...");
        Cyb_Vec3 a = {4, 5, 6};
        Cyb_Vec3 b = {1, 2, 3};
        Cyb_Vec3 c;
        Cyb_AddVec3(&c, &a, &b);
        
        if(c.x != 5 || c.y != 7 || c.z != 9)
        {
            puts("failed");
            return 1;
        }
    }
    
    //Test 3D vector subtraction
    {
        puts("Testing 3D vector subtraction...");
        Cyb_Vec3 a = {4, 5, 6};
        Cyb_Vec3 b = {1, 2, 3};
        Cyb_Vec3 c;
        Cyb_SubVec3(&c, &a, &b);
        
        if(c.x != 3 || c.y != 3 || c.z != 3)
        {
            puts("failed");
            return 1;
        }
    }
    
    //Test 3D vector multiplication
    {
        puts("Testing 3D vector multiplication...");
        Cyb_Vec3 a = {4, 5, 6};
        Cyb_Vec3 b = {1, 2, 3};
        Cyb_Vec3 c;
        Cyb_MulVec3(&c, &a, &b);
        
        if(c.x != 4 || c.y != 10 || c.z != 18)
        {
            puts("failed");
            return 1;
        }
    }
    
    //Test 3D vector division
    {
        puts("Testing 3D vector division...");
        Cyb_Vec3 a = {4, 5, 6};
        Cyb_Vec3 b = {1, 2, 3};
        Cyb_Vec3 c;
        Cyb_DivVec3(&c, &a, &b);
        
        if(c.x != 4 || c.y != 2.5f || c.z != 2)
        {
            puts("failed");
            return 1;
        }
    }
    
    //Test 4D vector addition
    {
        puts("Testing 4D vector addition...");
        Cyb_Vec4 a = {5, 6, 7, 8};
        Cyb_Vec4 b = {1, 2, 3, 4};
        Cyb_Vec4 c;
        Cyb_AddVec4(&c, &a, &b);
        
        if(c.x != 6 || c.y != 8 || c.z != 10 || c.w != 12)
        {
            puts("failed");
            return 1;
        }
    }
    
    //Test 4D vector subtraction
    {
        puts("Testing 4D vector subtraction...");
        Cyb_Vec4 a = {5, 6, 7, 8};
        Cyb_Vec4 b = {1, 2, 3, 4};
        Cyb_Vec4 c;
        Cyb_SubVec4(&c, &a, &b);
        
        if(c.x != 4 || c.y != 4 || c.z != 4 || c.w != 4)
        {
            puts("failed");
            return 1;
        }
    }
    
    //Test 4D vector multiplication
    {
        puts("Testing 4D vector multiplication...");
        Cyb_Vec4 a = {5, 6, 7, 8};
        Cyb_Vec4 b = {1, 2, 3, 4};
        Cyb_Vec4 c;
        Cyb_MulVec4(&c, &a, &b);
        
        if(c.x != 5 || c.y != 12 || c.z != 21 || c.w != 32)
        {
            puts("failed");
            return 1;
        }
    }
    
    //Test 4D vector division
    {
        puts("Testing 4D vector division...");
        Cyb_Vec4 a = {5, 6, 7, 8};
        Cyb_Vec4 b = {1, 2, 3, 4};
        Cyb_Vec4 c;
        Cyb_DivVec4(&c, &a, &b);
        
        if(c.x != 5 || c.y != 3 || (c.z < 2.3f || c.z > 2.4f) || c.w != 2)
        {
            puts("failed");
            return 1;
        }
    }
    
    return 0;
}


int TestCybMatrices(void)
{}


int main(int argc, char **argv)
{
    //Test vectors
    if(TestCybVectors())
    {
        return 1;
    }
    
    puts("done");
    return 0;
}