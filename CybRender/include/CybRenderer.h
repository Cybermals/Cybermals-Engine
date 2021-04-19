#ifndef CYBRENDERER_H
#define CYBRENDERER_H

/** @file
 * @brief CybRender - Renderer API
 */
 
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
 
#include "CybCommon.h"

 
#ifdef __cplusplus
extern "C" {
#endif

//Types
//=================================================================================
typedef struct Cyb_Renderer Cyb_Renderer;


//Structures
//=================================================================================
typedef struct
{
    //Shader Functions
    PFNGLCREATESHADERPROC CreateShader;
    PFNGLDELETESHADERPROC DeleteShader;
    PFNGLSHADERSOURCEPROC ShaderSource;
    PFNGLCOMPILESHADERPROC CompileShader;
    PFNGLGETSHADERIVPROC GetShaderiv;
    PFNGLGETSHADERINFOLOGPROC GetShaderInfoLog;
    
    //Program Functions
    PFNGLCREATEPROGRAMPROC CreateProgram;
    PFNGLDELETEPROGRAMPROC DeleteProgram;
    PFNGLATTACHSHADERPROC AttachShader;
    PFNGLLINKPROGRAMPROC LinkProgram;
    PFNGLGETPROGRAMIVPROC GetProgramiv;
    PFNGLGETPROGRAMINFOLOGPROC GetProgramInfoLog;
    PFNGLVALIDATEPROGRAMPROC ValidateProgram;
    PFNGLUSEPROGRAMPROC UseProgram;
    PFNGLGETUNIFORMLOCATIONPROC GetUniformLocation;
    PFNGLUNIFORMMATRIX4FVPROC UniformMatrix4fv;
    PFNGLUNIFORM1IPROC Uniform1i;
    PFNGLUNIFORM3FVPROC Uniform3fv;
    PFNGLUNIFORM1FPROC Uniform1f;
    
    //Buffer Functions
    PFNGLGENBUFFERSPROC GenBuffers;
    PFNGLDELETEBUFFERSPROC DeleteBuffers;
    PFNGLBINDBUFFERPROC BindBuffer;
    PFNGLBUFFERDATAPROC BufferData;
    PFNGLMAPBUFFERPROC MapBuffer;
    PFNGLUNMAPBUFFERPROC UnmapBuffer;
    
    //Vertex Array Functions
    PFNGLGENVERTEXARRAYSPROC GenVertexArrays;
    PFNGLDELETEVERTEXARRAYSPROC DeleteVertexArrays;
    PFNGLBINDVERTEXARRAYPROC BindVertexArray;
    
    //Vertex Attrib Functions
    PFNGLENABLEVERTEXATTRIBARRAYPROC EnableVertexAttribArray;
    PFNGLDISABLEVERTEXATTRIBARRAYPROC DisableVertexAttribArray;
    PFNGLVERTEXATTRIBPOINTERPROC VertexAttribPointer;
    
    //Texture Functions
    PFNGLACTIVETEXTUREPROC ActiveTexture;
} Cyb_GLExtAPI;


//Functions
//=================================================================================
CYBAPI Cyb_Renderer *Cyb_CreateRenderer(SDL_Window *window);
CYBAPI void Cyb_SelectRenderer(Cyb_Renderer *renderer);
CYBAPI void Cyb_SetRenderBGColor(Cyb_Renderer *renderer, float r, float g, float b,
    float a);
CYBAPI void Cyb_RenderClear(Cyb_Renderer *renderer);
CYBAPI Cyb_GLExtAPI *Cyb_GetGLExtAPI(Cyb_Renderer *renderer);
CYBAPI void Cyb_RenderPresent(Cyb_Renderer *renderer);


#ifdef __cplusplus
}
#endif

#endif