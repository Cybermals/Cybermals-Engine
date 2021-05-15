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

/** @addtogroup CybRender
 * @brief Cybermals Engine - Renderer Subsystem
 * @{
 */

//Types
//=================================================================================
/** @brief Renderer type.
 */
typedef struct Cyb_Renderer Cyb_Renderer;


//Structures
//=================================================================================
/** @brief OpenGL extension functions interface.
 */
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
    PFNGLBINDATTRIBLOCATIONPROC BindAttribLocation;
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
/** @brief Create a new renderer for a window.
 *
 * @param window Pointer to the window.
 *
 * @return Pointer to the renderer.
 */
CYBAPI Cyb_Renderer *Cyb_CreateRenderer(SDL_Window *window);

/** @brief Select a renderer to use.
 *
 * @param renderer Pointer to the renderer.
 */
CYBAPI void Cyb_SelectRenderer(Cyb_Renderer *renderer);

/** @brief Set the background color to be used by the given renderer.
 *
 * @param renderer Pointer to the renderer.
 * @param r The red value.
 * @param g The green value.
 * @param b The blue value.
 * @param a The alpha value.
 */
CYBAPI void Cyb_SetRenderBGColor(Cyb_Renderer *renderer, float r, float g, float b,
    float a);
    
/** @brief Clear the window associated with the given renderer.
 *
 * @param renderer Pointer to the renderer.
 */
CYBAPI void Cyb_RenderClear(Cyb_Renderer *renderer);

/** @brief Get the OpenGL extension functions associated with the given renderer.
 *
 * @param renderer Pointer to the renderer.
 *
 * @return Pointer to the OpenGL extension function interface.
 */
CYBAPI Cyb_GLExtAPI *Cyb_GetGLExtAPI(Cyb_Renderer *renderer);

/** @brief Present the content that has been rendered by swapping the front and
 * back buffers.
 *
 * @param renderer Pointer to the renderer.
 */
CYBAPI void Cyb_RenderPresent(Cyb_Renderer *renderer);

/**
 * @}
 */


#ifdef __cplusplus
}
#endif

#endif