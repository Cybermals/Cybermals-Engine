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
{
    //Test identity matrix
    puts("Testing identity matrix...");
    Cyb_Mat4 i;
    Cyb_Identity(&i);
    
    if(i.a != 1 || i.f != 1 || i.k != 1 || i.p != 1)
    {
        puts("failed");
        return 1;
    }
    
    //Test translation matrix
    puts("Testing translation matrix...");
    Cyb_Mat4 t;
    Cyb_Translate(&t, 4, 5, 6);
    
    if(t.d != 4 || t.h != 5 || t.l != 6)
    {
        puts("failed");
        return 1;
    }
    
    //Test rotation matrix
    puts("Testing rotation matrix...");
    Cyb_Mat4 r;
    Cyb_Rotate(&r, 0, 0, 90);
    
    //Test scaling matrix
    puts("Testing scaling matrix...");
    Cyb_Mat4 s;
    Cyb_Scale(&s, 4, 5, 6);
    
    if(s.a != 4 || s.f != 5 || s.k != 6)
    {
        puts("failed");
        return 1;
    }
    
    //Test ortho projection matrix
    puts("Testing ortho projection matrix...");
    Cyb_Mat4 o;
    Cyb_Ortho(&o, -1, 1, 1, -1, 1, -1);
    
    //Test perspective projection matrix
    puts("Testing perspective projection matrix...");
    Cyb_Mat4 p;
    Cyb_Perspective(&p, 45, 800.0f / 600.0f, .1f, 1000);
    
    //Test matrix multiplication
    puts("Testing matrix multiplication...");
    Cyb_Vec3 v = {1, 1, 1};
    Cyb_Vec3 v2;
    Cyb_Mat4 tmp;
    Cyb_Mat4 mv;
    Cyb_MulMat4(&tmp, &t, &r);
    Cyb_MulMat4(&mv, &tmp, &s);
    Cyb_Transform(&v2, &mv, &v);
    
    if((int)v2.x != -1 || (int)v2.y != 9 || (int)v2.z != 12)
    {
        printf("[%f, %f, %f]\n", v2.x, v2.y, v2.z);
        puts("failed");
        return 1;
    }
    
    v.z = -v.z;
    Cyb_Transform(&v2, &p, &v);
    printf("Projected point (1, 1, 1) = (%f, %f, %f)\n", v2.x, v2.y, v2.z);
    return 0;
}


int TestCybBoxes(void)
{
    //Test collision detection
    puts("Testing collision detection...");
    Cyb_Box a = {{0, 0, 0}, {10, 10, 10}};
    Cyb_Box b = {{5, 5, 5}, {2, 2, 2}};
    Cyb_Box c = {{-5, -5, -5}, {2, 2, 2}};
    Cyb_Vec3 p = {0, 0, 0};
    
    if(!Cyb_HasIntersection(&a, &b) || Cyb_HasIntersection(&b, &c) ||
        !Cyb_HasIntersection(&a, &c) || !Cyb_PointInBox(&p, &a) ||
        Cyb_PointInBox(&p, &b))
    {
        puts("failed");
        return 1;
    }
    
    //Test bounding box rotation
    puts("Testing bounding box rotation...");
    Cyb_Box d;
    Cyb_RotateBox(&d, &a, 0, 0, 45);
    printf("center = (%f, %f, %f)\nsize = (%f, %f, %f)\n", d.center.x, d.center.y,
        d.center.z, d.size.x, d.size.y, d.size.z);
    return 0;
}


int main(int argc, char **argv)
{
    //Test vectors
    if(TestCybVectors())
    {
        return 1;
    }
    
    //Test matrices
    if(TestCybMatrices())
    {
        return 1;
    }
    
    //Test bounding boxes
    if(TestCybBoxes())
    {
        return 1;
    }
    
    puts("done");
    return 0;
}