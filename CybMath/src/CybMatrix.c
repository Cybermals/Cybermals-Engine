/*
CybMath - Matrix API
*/

#include "CybMatrix.h"


//Functions
//=================================================================================
void Cyb_MulMat4(Cyb_Mat4 *c, const Cyb_Mat4 *a, const Cyb_Mat4 *b)
{
    //Row 1
    c->a = a->a * b->a + a->b * b->e + a->c * b->i + a->d * b->m;
    c->b = a->a * b->b + a->b * b->f + a->c * b->j + a->d * b->n;
    c->c = a->a * b->c + a->b * b->g + a->c * b->k + a->d * b->o;
    c->d = a->a * b->d + a->b * b->h + a->c * b->l + a->d * b->p;
    
    //Row 2
    c->e = a->e * b->a + a->f * b->e + a->g * b->i + a->h * b->m;
    c->f = a->e * b->b + a->f * b->f + a->g * b->j + a->h * b->n;
    c->g = a->e * b->c + a->f * b->g + a->g * b->k + a->h * b->o;
    c->h = a->e * b->d + a->f * b->h + a->g * b->l + a->h * b->p;
    
    //Row 3
    c->i = a->i * b->a + a->j * b->e + a->k * b->i + a->l * b->m;
    c->j = a->i * b->b + a->j * b->f + a->k * b->j + a->l * b->n;
    c->k = a->i * b->c + a->j * b->g + a->k * b->k + a->l * b->o;
    c->l = a->i * b->d + a->j * b->h + a->k * b->l + a->l * b->p;
    
    //Row 4
    c->m = a->m * b->a + a->n * b->e + a->o * b->i + a->p * b->m;
    c->n = a->m * b->b + a->n * b->f + a->o * b->i + a->p * b->n;
    c->o = a->m * b->c + a->n * b->g + a->o * b->i + a->p * b->o;
    c->p = a->m * b->d + a->n * b->h + a->o * b->i + a->p * b->p;
}


void Cyb_Transpose(Cyb_Mat4 *out, const Cyb_Mat4 *in)
{
    out->a = in->a;
    out->b = in->e;
    out->c = in->i;
    out->d = in->m;
    
    out->e = in->b;
    out->f = in->f;
    out->g = in->j;
    out->h = in->n;
    
    out->i = in->c;
    out->j = in->g;
    out->k = in->k;
    out->l = in->o;
    
    out->m = in->d;
    out->n = in->h;
    out->o = in->l;
    out->p = in->p;
}


//Based on the matrix determinant code in Assimp. Adapted to work with the
//Cybermals column-major matrices.
float Cyb_Determinant(const Cyb_Mat4 *m)
{
    return m->a * m->f * m->k * m->p -
        m->a * m->f * m->l * m->o +
        m->a * m->g * m->l * m->n -
        m->a * m->g * m->j * m->p +
        m->a * m->h * m->j * m->o -
        m->a * m->h * m->k * m->n -
        m->b * m->g * m->l * m->m +
        m->b * m->g * m->i * m->p -
        m->b * m->h * m->i * m->o +
        m->b * m->h * m->k * m->m -
        m->b * m->e * m->k * m->p +
        m->b * m->e * m->l * m->o +
        m->c * m->h * m->i * m->n -
        m->c * m->h * m->j * m->m +
        m->c * m->e * m->j * m->p -
        m->c * m->e * m->l * m->n +
        m->c * m->f * m->l * m->m -
        m->c * m->f * m->i * m->p -
        m->d * m->e * m->j * m->o +
        m->d * m->e * m->k * m->n -
        m->d * m->f * m->k * m->m +
        m->d * m->f * m->i * m->o -
        m->d * m->g * m->i * m->n +
        m->d * m->g * m->j * m->m;
}


//Based on the matrix inversion code in Assimp. Adapted to work with the
//Cybermals column-major matrices.
void Cyb_Invert(Cyb_Mat4 *out, const Cyb_Mat4 *in)
{
    //Calculate the determinant
    const float det = Cyb_Determinant(in);
    
    if(det == 0.0f)
    {
        return;
    }
    
    //Calculate the inverse determinant
    const float invdet = 1.0f / det;
    
    //Calculate the inverse matrix
    out->a = invdet * (in->f * (in->k * in->p - in->l * in->o) + in->g * (in->l * in->n - in->j * in->p) + in->h * (in->j * in->o - in->k * in->n));
    out->b = -invdet * (in->b * (in->k *in->p - in->l * in->o) + in->c * (in->l * in->n - in->j * in->p) + in->d * (in->j * in->o - in->k * in->n));
    out->c = invdet * (in->b * (in->g * in->p - in->h * in->o) + in->c * (in->h * in->n - in->f * in->p) + in->d * (in->f * in->o - in->g * in->n));
    out->d = -invdet * (in->b * (in->g * in->l -in->h * in->k) + in->c * (in->h * in->j - in->f * in->l) + in->d * (in->f * in->k - in->g * in->j));
    
    out->e = -invdet * (in->e * (in->k * in->p - in->l * in->o) + in->g * (in->l * in->m - in->i * in->p) + in->h * (in->i * in->o - in->k * in->m));
    out->f = invdet * (in->a * (in->k * in->p - in->l * in->o) + in->c * (in->l * in->m - in->i * in->p) + in->d * (in->i * in->o - in->k * in->m));
    out->g = -invdet * (in->a * (in->g * in->p - in->h * in->o) + in->c * (in->h * in->m - in->e * in->p) + in->d *(in->e * in->o - in->g * in->m));
    out->h = invdet * (in->a * (in->g * in->l - in->h * in->k) + in->c * (in->h * in->i - in->e * in->l) + in->d * (in->e * in->k - in->g * in->i));
    
    out->i = invdet * (in->e * (in->j * in->p - in->l * in->n) + in->f * (in->l * in->m - in->i * in->p) + in->h * (in->i * in->n - in->j * in->m));
    out->j = -invdet * (in->a * (in->j * in->p - in->l * in->n) + in->b * (in->l * in->m - in->i * in->p) + in->d * (in->i * in->n - in->j * in->m));
    out->k = invdet * (in->a * (in->f * in->p - in->h * in->n) + in->b * (in->h * in->m - in->e * in->p) + in->d * (in->e * in->n - in->f * in->m));
    out->l = -invdet * (in->a * (in->f * in->l - in->h * in->j) + in->b * (in->h * in->i - in->e * in->l) + in->d * (in->e * in->j - in->f * in->i));
    
    out->m = -invdet * (in->e * (in->j * in->o - in->k * in->n) + in->f * (in->k * in->m - in->i * in->o) + in->g * (in->i * in->n - in->j * in->m));
    out->n = invdet * (in->a * (in->j * in->o - in->k * in->n) + in->b * (in->k * in->m - in->i * in->o) + in->c * (in->i * in->n - in->j * in->m));
    out->o = -invdet * (in->a * (in->f * in->o - in->g * in->n) + in->b * (in->g * in->m - in->e * in->o) + in->c * (in->e * in->n - in->f * in->m));
    out->p = invdet * (in->a * (in->f * in->k - in->g * in->j) + in->b * (in->g * in->i - in->e * in->k) + in->c * (in->e * in->j - in->f * in->i));
}


void Cyb_Transform(Cyb_Vec3 *c, const Cyb_Mat4 *a, const Cyb_Vec3 *b)
{
    c->x = a->a * b->x + a->b * b->y + a->c * b->z + 1.0f * a->d;
    c->y = a->e * b->x + a->f * b->y + a->g * b->z + 1.0f * a->h;
    c->z = a->i * b->x + a->j * b->y + a->k * b->z + 1.0f * a->l;
}


void Cyb_Identity(Cyb_Mat4 *m)
{
    //Row 1
    m->a = 1.0f;
    m->b = 0.0f;
    m->c = 0.0f;
    m->d = 0.0f;
    
    //Row 2
    m->e = 0.0f;
    m->f = 1.0f;
    m->g = 0.0f;
    m->h = 0.0f;
    
    //Row 3
    m->i = 0.0f;
    m->j = 0.0f;
    m->k = 1.0f;
    m->l = 0.0f;
    
    //Row 4
    m->m = 0.0f;
    m->n = 0.0f;
    m->o = 0.0f;
    m->p = 1.0f;
}


void Cyb_Translate(Cyb_Mat4 *m, float x, float y, float z)
{
    //Start with the identity matrix
    Cyb_Identity(m);
    
    //Modify column 4
    m->d = x;
    m->h = y;
    m->l = z;
}


void Cyb_Rotate(Cyb_Mat4 *m, float x, float y, float z, int rotOrder)
{
    //Generate X rotation matrix
    Cyb_Mat4 matX;
    Cyb_Identity(&matX);
    
    matX.f = cosf(radians(x));
    matX.g = -sinf(radians(x));
    matX.j = sinf(radians(x));
    matX.k = cosf(radians(x));
    
    //Generate Y rotation matrix
    Cyb_Mat4 matY;
    Cyb_Identity(&matY);
    
    matY.a = cosf(radians(y));
    matY.c = sinf(radians(y));
    matY.i = -sinf(radians(y));
    matY.k = cosf(radians(y));
    
    //Generate Z rotation matrix
    Cyb_Mat4 matZ;
    Cyb_Identity(&matZ);
    
    matZ.a = cosf(radians(z));
    matZ.b = -sinf(radians(z));
    matZ.e = sinf(radians(z));
    matZ.f = cosf(radians(z));
    
    //Multiply rotation matrices
    Cyb_Mat4 tmp;
    
    switch(rotOrder)
    {
        //XYZ?
    case CYB_ROT_XYZ:
        Cyb_MulMat4(&tmp, &matZ, &matY);
        Cyb_MulMat4(m, &tmp, &matX);
        break;
        
        //ZYX?
    case CYB_ROT_ZYX:
        Cyb_MulMat4(&tmp, &matX, &matY);
        Cyb_MulMat4(m, &tmp, &matZ);
        break;
        
        //ZXY?
    case CYB_ROT_ZXY:
        Cyb_MulMat4(&tmp, &matY, &matX);
        Cyb_MulMat4(m, &tmp, &matZ);
        break;
    }
}


void Cyb_Scale(Cyb_Mat4 *m, float x, float y, float z)
{
    //Start with the identity matrix
    Cyb_Identity(m);
    
    //Modify first 3 diagonal values
    m->a = x;
    m->f = y;
    m->k = z;
}


void Cyb_LookAt(Cyb_Mat4 *m, Cyb_Vec3 *pos, Cyb_Vec3 *right, Cyb_Vec3 *up, 
    Cyb_Vec3 *dir)
{
    //Generate position matrix
    Cyb_Mat4 posMat;
    Cyb_Translate(&posMat, -pos->x, -pos->y, -pos->z);
    
    //Generate vector matrix
    Cyb_Mat4 vecMat;
    Cyb_Identity(&vecMat);
    vecMat.a = right->x;
    vecMat.b = right->y;
    vecMat.c = right->z;
    vecMat.e = up->x;
    vecMat.f = up->y;
    vecMat.g = up->z;
    vecMat.i = dir->x;
    vecMat.j = dir->y;
    vecMat.k = dir->z;
    
    //Multiply position and vector matrices
    Cyb_MulMat4(m, &posMat, &vecMat);
}


void Cyb_Ortho(Cyb_Mat4 *m, float left, float right, float top, 
    float bottom, float near, float far)
{
    //Start with the identity matrix
    Cyb_Identity(m);
    
    //Modify first 3 diagonal values
    m->a = 2.0f / (right - left);
    m->f = 2.0f / (top - bottom);
    m->k = -2.0f / (far - near);
    
    //Modify column 4
    m->d = -(right + left) / (right - left);
    m->h = -(top + bottom) / (top - bottom);
    m->l = -(far + near) / (far - near);
}


void Cyb_Perspective(Cyb_Mat4 *m, float fov, float aspect, float near, 
    float far)
{
    //Start with the identity matrix
    Cyb_Identity(m);
    
    //Calculate f value
    float f = 1.0f / tanf(radians(fov) / 2.0f);
    
    //Modify first 3 diagonal values
    m->a = f / aspect;
    m->f = f;
    m->k = (far + near) / (near - far);
    
    //Modify column 3
    m->o = -1.0f;
    
    //Modify column 4
    m->l = (2.0f * far * near) / (near - far);
    m->p = 0.0f;
}