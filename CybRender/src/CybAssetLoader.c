/*
CybRender - Asset Loader API
*/

#include <string.h>

#include "CybAnimation.h"
#include "CybAnimChannel.h"
#include "CybArmature.h"
#include "CybAssetLoader.h"
#include "CybMaterial.h"
#include "CybMesh.h"
#include "CybTexture.h"


//Constants
//=================================================================================
const char *loadMeshSQL = "SELECT vert_count, vertices, normals, colors, uvs, index_count, indices FROM meshes WHERE name = ?;";
const char *loadMaterialSQL = "SELECT ambient, diffuse, specular, shininess FROM materials WHERE name = ?;";
const char *loadTextureSQL = "SELECT width, height, format, data FROM textures WHERE name = ?;";
const char *loadArmatureSQL = "SELECT vert_count, vgroups, vweights, bone_count, bones FROM armatures WHERE name = ?;";
const char *loadAnimationSQL = "SELECT channel_count, ticks_per_sec FROM animations WHERE name = ?;";
const char *loadAnimChannelSQL = "SELECT name, pos_key_count, pos_keys, rot_key_count, rot_keys, scl_key_count, scl_keys FROM anim_channels WHERE anim_id IN (SELECT id FROM animations WHERE name = ?);";


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
            sqlite3_finalize(loadTextureStmt);
            return NULL;
        }
        
        //Finalize SQL statements
        sqlite3_finalize(loadTextureStmt);
        return (Cyb_Object*)tex;
    }
    
        //Armature?
    case CYB_ARMATURE_ASSET:
    {
        //Compile SQL statements
        sqlite3_stmt *loadArmatureStmt = NULL;
        
        if(sqlite3_prepare_v2(db, loadArmatureSQL, -1, &loadArmatureStmt, NULL) !=
            SQLITE_OK)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", 
                "[CybRender] Failed to compile an SQL statement.");
            return NULL;
        }
        
        //Load the armature
        sqlite3_bind_text(loadArmatureStmt, 1, name, -1, NULL);
        
        if(sqlite3_step(loadArmatureStmt) != SQLITE_ROW)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s",
                "[CybRender] Failed to locate armature '%s' in the asset database.");
            sqlite3_finalize(loadArmatureStmt);
            return NULL;
        }
        
        Cyb_Armature *armature = Cyb_CreateArmature(renderer);
        
        if(!armature)
        {
            sqlite3_finalize(loadArmatureStmt);
            return NULL;
        }
        
        Cyb_UpdateArmature(
            renderer,
            armature,
            sqlite3_column_int(loadArmatureStmt, 0),
            sqlite3_column_blob(loadArmatureStmt, 1),
            sqlite3_column_blob(loadArmatureStmt, 2),
            sqlite3_column_int(loadArmatureStmt, 3),
            sqlite3_column_blob(loadArmatureStmt, 4)
        );
        
        //Finalize SQL statements
        sqlite3_finalize(loadArmatureStmt);
        return (Cyb_Object*)armature;
    }
    
        //Animation?
    case CYB_ANIMATION_ASSET:
    {
        //Compile SQL statements
        sqlite3_stmt *loadAnimationStmt = NULL;
        sqlite3_stmt *loadAnimChannelStmt = NULL;
        
        if(sqlite3_prepare_v2(db, loadAnimationSQL, -1, &loadAnimationStmt, NULL) !=
            SQLITE_OK)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s",
                "[CybRender] Failed to compile an SQL statement.");
            return NULL;
        }
        
        if(sqlite3_prepare_v2(db, loadAnimChannelSQL, -1, &loadAnimChannelStmt, 
            NULL) != SQLITE_OK)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s",
                "[CybRender] Failed to compile an SQL statement.");
            sqlite3_finalize(loadAnimationStmt);
            return NULL;
        }
        
        //Load animation
        sqlite3_bind_text(loadAnimationStmt, 1, name, -1, NULL);
        
        if(sqlite3_step(loadAnimationStmt) != SQLITE_ROW)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "[CybRender] Failed to locate animation '%s' in the asset database.",
                name);
            sqlite3_finalize(loadAnimationStmt);
            sqlite3_finalize(loadAnimChannelStmt);
            return NULL;
        }
        
        int channelCount = sqlite3_column_int(loadAnimationStmt, 0);
        double ticksPerSecond = sqlite3_column_double(loadAnimationStmt, 1);
        Cyb_Animation *anim = Cyb_CreateAnimation(ticksPerSecond);
        
        if(!anim)
        {
            return NULL;
        }
        
        //Load animation channels
        sqlite3_bind_text(loadAnimChannelStmt, 1, name, -1, NULL);
        
        for(int i = 0; i < channelCount; i++)
        {
            //Load next animation channel
            if(sqlite3_step(loadAnimChannelStmt) != SQLITE_ROW)
            {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                    "[CybRender] Failed to load animation channel %i.", i);
                Cyb_FreeObject((Cyb_Object**)&anim);
                sqlite3_finalize(loadAnimationStmt);
                sqlite3_finalize(loadAnimChannelStmt);
            }
        
            Cyb_AnimChannel *channel = Cyb_AddAnimChannel(anim);
            
            if(!channel)
            {
                Cyb_FreeObject((Cyb_Object**)&anim);
                sqlite3_finalize(loadAnimationStmt);
                sqlite3_finalize(loadAnimChannelStmt);
                return NULL;
            }
            
            Cyb_UpdateAnimChannel(
                channel,
                sqlite3_column_text(loadAnimChannelStmt, 0),
                sqlite3_column_int(loadAnimChannelStmt, 1),
                sqlite3_column_blob(loadAnimChannelStmt, 2),
                sqlite3_column_int(loadAnimChannelStmt, 3),
                sqlite3_column_blob(loadAnimChannelStmt, 4),
                sqlite3_column_int(loadAnimChannelStmt, 5),
                sqlite3_column_blob(loadAnimChannelStmt, 6)
            );
        }
        
        //Finalize SQL statements
        sqlite3_finalize(loadAnimationStmt);
        sqlite3_finalize(loadAnimChannelStmt);
        return (Cyb_Object*)anim;
    }
    }
    
    return NULL;
}