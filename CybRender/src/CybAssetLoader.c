/*
CybRender - Asset Loader API
*/

#include "CybAssetLoader.h"


//Constants
//=================================================================================
const char *loadMeshSQL = "SELECT * FROM meshes WHERE name=?;";


//Functions
//=================================================================================
Cyb_Object *Cyb_LoadAsset(Cyb_Renderer *renderer, const char *filename,
    const char *name, int type)
{
    //Open SQL database
    sqlite3 *db;
    
    if(sqlite3_open_v2(filename, &db, SQLITE_OPEN_READONLY, NULL) != SQLITE_OK)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
            "[CybRender] Failed to open asset database '%s'.", filename);
        return NULL;
    }
    
    //Load the asset
    Cyb_Object *asset = Cyb_LoadAsset_DB(renderer, db, name, type);
    
    //Close SQL database
    sqlite3_close_v2(db);
    return asset;
}


Cyb_Object *Cyb_LoadAsset_DB(Cyb_Renderer *renderer, sqlite3 *db, 
    const char *name, int type)
{
    //Choose asset type
    switch(type)
    {
        //Mesh asset?
    case CYB_MESH_ASSET:
    {
        //Compile SQL statements
        sqlite3_stmt *loadMeshStmt = NULL;
        
        if(sqlite3_prepare_v2(db, loadMeshSQL, -1, &loadMeshStmt, NULL) != 
            SQLITE_OK)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s",
                "[CybRender] Failed to compile an SQL statement.");
            return NULL;
        }
        
        //Load the mesh
        sqlite3_bind_text(loadMeshStmt, 1, name, -1, NULL);
        
        if(sqlite3_step(loadMeshStmt) != SQLITE_ROW)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s",
                "[CybRender] Failed to locate resource '%s' in the asset database.");
            sqlite3_finalize(loadMeshStmt);
            return NULL;
        }
        
        Cyb_Mesh *mesh = Cyb_CreateMesh(renderer);
        
        if(!mesh)
        {
            sqlite3_finalize(loadMeshStmt);
            return NULL;
        }
        
        Cyb_UpdateMesh(
            renderer,
            mesh,
            sqlite3_column_int(loadMeshStmt, 1),
            sqlite3_column_blob(loadMeshStmt, 2),
            sqlite3_column_blob(loadMeshStmt, 3),
            sqlite3_column_blob(loadMeshStmt, 4),
            sqlite3_column_blob(loadMeshStmt, 5),
            sqlite3_column_int(loadMeshStmt, 6),
            sqlite3_column_blob(loadMeshStmt, 7)
        );
        
        //Finalize SQL statements
        sqlite3_finalize(loadMeshStmt);
        return (Cyb_Object*)mesh;
    }
    }
    
    return NULL;
}