/*
CybRender - Asset Loader API
*/

#include <string.h>

#include "CybAssetLoader.h"
#include "CybMaterial.h"
#include "CybMesh.h"
#include "CybTexture.h"


//Constants
//=================================================================================
const char *loadMeshSQL = "SELECT vert_count, vertices, normals, colors, uvs, index_count, indices FROM meshes WHERE name=?;";
const char *loadMaterialSQL = "SELECT ambient, diffuse, specular, shininess FROM materials WHERE name=?;";
const char *loadTextureSQL = "SELECT width, height, format, data FROM textures WHERE name=?;";


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
                "[CybRender] Failed to locate mesh '%s' in the asset database.");
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
            sqlite3_column_int(loadMeshStmt, 0),
            sqlite3_column_blob(loadMeshStmt, 1),
            sqlite3_column_blob(loadMeshStmt, 2),
            sqlite3_column_blob(loadMeshStmt, 3),
            sqlite3_column_blob(loadMeshStmt, 4),
            sqlite3_column_int(loadMeshStmt, 5),
            sqlite3_column_blob(loadMeshStmt, 6)
        );
        
        //Finalize SQL statements
        sqlite3_finalize(loadMeshStmt);
        return (Cyb_Object*)mesh;
    }
    
        //Material asset?
    case CYB_MATERIAL_ASSET:
    {
        //Compile SQL statements
        sqlite3_stmt *loadMaterialStmt = NULL;
        
        if(sqlite3_prepare_v2(db, loadMaterialSQL, -1, &loadMaterialStmt, NULL) !=
            SQLITE_OK)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s",
                "[CybRender] Failed to compile an SQL statement.");
            return NULL;
        }
        
        //Load the material
        sqlite3_bind_text(loadMaterialStmt, 1, name, -1, NULL);
        
        if(sqlite3_step(loadMaterialStmt) != SQLITE_ROW)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s",
                "[CybRender] Failed to locate material '%s' in the asset database.");
            sqlite3_finalize(loadMaterialStmt);
            return NULL;
        }
        
        Cyb_Material *mat = Cyb_CreateMaterial();
        
        if(!mat)
        {
            sqlite3_finalize(loadMaterialStmt);
            return NULL;
        }
        
        memcpy(&mat->ambient, sqlite3_column_blob(loadMaterialStmt, 0), 
            sizeof(mat->ambient));
        memcpy(&mat->diffuse, sqlite3_column_blob(loadMaterialStmt, 1),
            sizeof(mat->diffuse));
        memcpy(&mat->specular, sqlite3_column_blob(loadMaterialStmt, 2),
            sizeof(mat->specular));
        mat->shininess = sqlite3_column_double(loadMaterialStmt, 3);
        
        //Finalize SQL statements
        sqlite3_finalize(loadMaterialStmt);
        return (Cyb_Object*)mat;
    }
    
        //Texture?
    case CYB_TEXTURE_ASSET:
    {
        //Compile SQL statements
        sqlite3_stmt *loadTextureStmt = NULL;
        
        if(sqlite3_prepare_v2(db, loadTextureSQL, -1, &loadTextureStmt, NULL) != 
            SQLITE_OK)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s",
                "[CybRender] Failed to compile an SQL statement.");
            return NULL;
        }
        
        //Load the texture
        sqlite3_bind_text(loadTextureStmt, 1, name, -1, NULL);
        
        if(sqlite3_step(loadTextureStmt) != SQLITE_ROW)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s",
                "[CybRender] Failed to locate texture '%s' in the asset database.");
            sqlite3_finalize(loadTextureStmt);
            return NULL;
        }
        
        SDL_RWops *file = SDL_RWFromConstMem(
            sqlite3_column_blob(loadTextureStmt, 3),
            sqlite3_column_bytes(loadTextureStmt, 3)
        );
        
        if(!file)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s",
                "[CybRender] Failed to open texture blob.");
            sqlite3_finalize(loadTextureStmt);
            return NULL;
        }
        
        Cyb_Texture *tex = Cyb_LoadTextureRW(renderer, file, TRUE, name);
        
        if(!tex)
        {
            return NULL;
        }
        
        //Finalize SQL statements
        sqlite3_finalize(loadTextureStmt);
        return (Cyb_Object*)tex;
    }
    }
    
    return NULL;
}