#ifndef CYBASSETLOADER_H
#define CYBASSETLOADER_H

/** @file
 * @brief CybRender - Asset Loader API
 */
 
#include <sqlite3.h> 

#include "CybObjects.h"
#include "CybRenderer.h"

 
#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup CybRender
 * @brief Cybermals Engine - Renderer Subsystem
 * @{
 */

//Enums
//================================================================================
/** @brief Asset type enum.
 */
enum Cyb_AssetType
{
    CYB_MESH_ASSET,     /**< Mesh asset. */
    CYB_MATERIAL_ASSET, /**< Material asset. */
    CYB_TEXTURE_ASSET,  /**< Texture asset. */
    CYB_ARMATURE_ASSET, /**< Armature asset. */
    CYB_ANIMATION_ASSET /**< Animation asset. */
};


//Functions
//================================================================================
/** @brief Load an asset from an asset database.
 *
 * @param renderer Pointer to the renderer.
 * @param filename The path to the asset database.
 * @param name The name of the asset to load.
 * @param type The type of the asset to load.
 *
 * @return Pointer to the loaded asset or NULL.
 */
CYBAPI Cyb_Object *Cyb_LoadAsset(Cyb_Renderer *renderer, const char *filename,
    const char *name, int type);
    
/** @brief Load an asset from an open asset database.
 *
 * @param renderer Pointer to the renderer.
 * @param db Pointer to the open asset database.
 * @param name The name of the asset to load.
 * @param type The type of the asset to load.
 *
 * @return Pointer to the loaded asset or NULL.
 */
CYBAPI Cyb_Object *Cyb_LoadAsset_DB(Cyb_Renderer *renderer, sqlite3 *db, 
    const char *name, int type);
    
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif