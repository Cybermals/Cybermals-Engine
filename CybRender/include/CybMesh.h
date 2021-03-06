#ifndef CYBMESH_H
#define CYBMESH_H

/** @file
 * @brief CybRender - Mesh API
 */
 
#include "CybCommon.h"
#include "CybMath.h"
#include "CybRenderer.h"

#define Cyb_DrawMesh(renderer, mesh) Cyb_DrawMeshes(renderer, mesh, 1)

 
#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup CybRender
 * @brief Cybermals Engine - Renderer Subsystem
 * @{
 */
 
//Enums
//=================================================================================
/** @brief Vertex attribute indices.
 */
enum Cyb_VertexAttribs
{
    CYB_ATTRIB_POS,                /**< Vertex position attrib. */
    CYB_ATTRIB_NORM,               /**< Vertex normal attrib. */
    CYB_ATTRIB_TANGENT,            /**< Tangent vector attrib. */
    CYB_ATTRIB_COLOR,              /**< Vertex color attrib. */
    CYB_ATTRIB_UV,                 /**< Vertex uv attrib. */
    CYB_ATTRIB_GROUP,              /**< Vertex group attrib. */
    CYB_ATTRIB_WEIGHT,             /**< Vertex weight attrib. */
    CYB_ATTRIB_INSTANCE_MAT_MODEL, /**< Instance model matrix. */
    CYB_ATTRIB_INSTANCE_MAT_NORM   /**< Instance normal matrix. */
};


//Types
//=================================================================================
/** @brief Mesh object.
 */
typedef struct Cyb_Mesh Cyb_Mesh;


//Structures
//=================================================================================
/** @brief Vertex.
 */
typedef struct
{
    Cyb_Vec3 pos; /**< Vertex position. */
} Cyb_VertexV;


/** @brief Vertex with normal.
 */
typedef struct
{
    Cyb_Vec3 pos;     /**< Vertex position. */
    Cyb_Vec3 norm;    /**< Vertex normal. */
    Cyb_Vec3 tangent; /**< Tangent vector. */
} Cyb_VertexVN;


/** @brief Vertex with normal and color.
 */
typedef struct
{
    Cyb_Vec3 pos;     /**< Vertex position. */
    Cyb_Vec3 norm;    /**< Vertex normal. */
    Cyb_Vec3 tangent; /**< Tangent vector. */
    Cyb_Vec4 color;   /**< Vertex color. */
} Cyb_VertexVNC;


/** @brief Vertex with normal and texture coordinate.
 */
typedef struct
{
    Cyb_Vec3 pos;     /**< Vertex position. */
    Cyb_Vec3 norm;    /**< Vertex normal. */
    Cyb_Vec3 tangent; /**< Tangent vector. */
    Cyb_Vec2 uv;      /**< Vertex texture coordinate. */
} Cyb_VertexVNT;


/** @brief Vertex with normal, color, and texture coordinate.
 */
typedef struct
{
    Cyb_Vec3 pos;     /**< Vertex position. */
    Cyb_Vec3 norm;    /**< Vertex normal. */
    Cyb_Vec3 tangent; /**< Tangent vector. */
    Cyb_Vec4 color;   /**< Vertex color. */
    Cyb_Vec2 uv;      /**< Vertex texture coordinate. */
} Cyb_VertexVNCT;


//Enums
//=================================================================================
/** @brief Vertex type.
 */
enum Cyb_VertexType
{
    CYB_VERTEX_UNKNOWN, /**< Unknown vertex type. */
    CYB_VERTEX_V,       /**< Vertex. */
    CYB_VERTEX_VN,      /**< Vertex with normal. */
    CYB_VERTEX_VNC,     /**< Vertex with normal and color. */
    CYB_VERTEX_VNT,     /**< Vertex with normal and texture coordinate. */
    CYB_VERTEX_VNCT     /**< Vertex with normal, color, and texture coordinate. */
};


//Functions
//=================================================================================
/** @brief Create a new mesh.
 *
 * @param renderer Pointer to the renderer.
 *
 * @return Pointer to the mesh.
 */
CYBAPI Cyb_Mesh *Cyb_CreateMesh(Cyb_Renderer *renderer);

/** @brief Update mesh data.
 *
 * @param renderer Pointer to the renderer.
 * @param mesh Pointer to the mesh.
 * @param vertCount The number of vertices.
 * @param verts The vertex positions (required).
 * @param norms The vertex normals (optional; required if colors or uvs are given).
 * @param tangents The tangent vectors (optional).
 * @param colors The vertex colors (optional).
 * @param uvs The vertex texture coordinates (optional).
 * @param indexCount The number of indices.
 * @param indices The element indices.
 *
 * @return CYB_NO_ERROR on success.
 */
CYBAPI int Cyb_UpdateMesh(Cyb_Renderer *renderer, Cyb_Mesh *mesh, int vertCount, 
    const Cyb_Vec3 *verts, const Cyb_Vec3 *norms, const Cyb_Vec3 *tangents, 
    const Cyb_Vec4 *colors, const Cyb_Vec2 *uvs, int indexCount, 
    const unsigned int *indices);
    
/** @brief Draw multiple instances of the same mesh.
 *
 * @param renderer Pointer to the renderer.
 * @param mesh Pointer to the mesh.
 * @param count The number of instances to draw.
 */
CYBAPI void Cyb_DrawMeshes(Cyb_Renderer *renderer, Cyb_Mesh *mesh, int count);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif