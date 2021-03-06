#ifndef CYBSHADER_H
#define CYBSHADER_H

/** @file
 * @brief CybRender - Shader API
 */
 
#include "CybCommon.h"
#include "CybMath.h"
#include "CybRenderer.h"

#define Cyb_LoadShader(renderer, filename) \
    Cyb_LoadShaderRW(renderer, SDL_RWFromFile(filename, "r"), TRUE, filename)

 
#ifdef __cplusplus
extern "C" {
#endif

//Types
//=================================================================================
typedef struct Cyb_Shader Cyb_Shader;


//Functions
//=================================================================================
CYBAPI Cyb_Shader *Cyb_LoadShaderRW(Cyb_Renderer *renderer, SDL_RWops *file, 
    int doClose, const char *id);
CYBAPI void Cyb_SelectShader(Cyb_Renderer *renderer, Cyb_Shader *shader);
CYBAPI void Cyb_SetMatrices(Cyb_Renderer *renderer, Cyb_Shader *shader,
    const char *name, int count, const Cyb_Mat4 *matrices);
CYBAPI void Cyb_SetMatrix(Cyb_Renderer *renderer, Cyb_Shader *shader,
    const char *name, const Cyb_Mat4 *matrix);
CYBAPI void Cyb_SetTexture(Cyb_Renderer *renderer, Cyb_Shader *shader,
    const char *name, int texUnit);
CYBAPI void Cyb_SetVec3(Cyb_Renderer *renderer, Cyb_Shader *shader,
    const char *name, const Cyb_Vec3 *v);
CYBAPI void Cyb_SetFloat(Cyb_Renderer *renderer, Cyb_Shader *shader,
    const char *name, float f);

#ifdef __cplusplus
}
#endif

#endif