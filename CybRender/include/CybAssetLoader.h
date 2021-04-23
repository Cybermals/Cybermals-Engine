#ifndef CYBASSETLOADER_H
#define CYBASSETLOADER_H

/** @file
 * @brief CybRender - Asset Loader API
 */
 
#include <sqlite3.h> 

#include "CybObjects.h"
#include "CybMesh.h"
#include "CybRenderer.h"

 
#ifdef __cplusplus
extern "C" {
#endif

//Enums
//================================================================================
enum Cyb_AssetType
{
    CYB_MESH_ASSET
};


//Functions
//================================================================================
CYBAPI Cyb_Object *Cyb_LoadAsset(Cyb_Renderer *renderer, const char *filename,
    const char *name, int type);
CYBAPI Cyb_Object *Cyb_LoadAsset_DB(Cyb_Renderer *renderer, sqlite3 *db, 
    const char *name, int type);

#ifdef __cplusplus
}
#endif

#endif