/*
Cybermals Engine - Asset Manager Tool
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sqlite3.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "CybKeyframes.h"
#include "CybBone.h"
#include "CybCommon.h"
#include "CybMath.h"

#define APP_TITLE "Cybermals Engine Asset Manager Tool"
#define APP_VERSION "1.0.0"

#ifndef TRUE
    #define TRUE  1
    #define FALSE 0
#endif


//Constants
//===========================================================================
const char *cmdRef = "Commands\n"
"========\n"
"help - display this command reference\n"
"quit - exit this program\n"
"open filename - open a new or existing asset database\n"
"list assets - list the assets in the open database\n"
"add mesh filename - add the contents of a mesh file to the open database\n"
"add texture filename - add a texture to the open database";

const char *initSQL = "PRAGMA foreign_keys=ON;\n"
"\n"
"BEGIN TRANSACTION;\n"
"    CREATE TABLE IF NOT EXISTS meshes(\n"
"        id INTEGER PRIMARY KEY,\n"
"        name varchar(256) UNIQUE,\n"
"        vert_count INT,\n"
"        vertices BLOB,\n"
"        normals BLOB,\n"
"        colors BLOB,\n"
"        uvs BLOB,\n"
"        index_count INT,\n"
"        indices BLOB,\n"
"        has_bones BOOL\n"
"    );\n"
"\n"
"    CREATE TABLE IF NOT EXISTS textures(\n"
"        id INTEGER PRIMARY KEY,\n"
"        name varchar(256) UNIQUE,\n"
"        width INT,\n"
"        height INT,\n"
"        format varchar(16),\n"
"        data BLOB"
"    );\n"
"\n"
"    CREATE TABLE IF NOT EXISTS materials(\n"
"        id INTEGER PRIMARY KEY,\n"
"        name varchar(256) UNIQUE,\n"
"        ambient BLOB,\n"
"        diffuse BLOB,\n"
"        specular BLOB,\n"
"        shininess REAL"
"    );\n"
"\n"
"    CREATE TABLE IF NOT EXISTS armatures(\n"
"        id INTEGER PRIMARY KEY,\n"
"        name varchar(256) UNIQUE,\n"
"        vert_count INT,\n"
"        vgroups BLOB,\n"
"        vweights BLOB,\n"
"        bone_count INT,\n"
"        bones BLOB\n"
"    );\n"
"\n"
"    CREATE TABLE IF NOT EXISTS animations(\n"
"        id INTEGER PRIMARY KEY,\n"
"        name varchar(256) UNIQUE,\n"
"        channel_count INT,\n"
"        ticks_per_sec REAL\n"
"    );\n"
"\n"
"    CREATE TABLE IF NOT EXISTS anim_channels(\n"
"        id INTEGER PRIMARY KEY,\n"
"        anim_id INTEGER,\n"
"        name varchar(32),\n"
"        pos_key_count INT,\n"
"        pos_keys BLOB,\n"
"        rot_key_count INT,\n"
"        rot_keys BLOB,\n"
"        scl_key_count INT,\n"
"        scl_keys BLOB,\n"
"        CONSTRAINT anim_channels_anim_id FOREIGN KEY(anim_id) REFERENCES animations(id) ON DELETE CASCADE"
"    );\n"
"END TRANSACTION;";

const char *listMeshesSQL = "SELECT name, vert_count, index_count FROM meshes ORDER BY name;";
const char *listTexturesSQL = "SELECT name, width, height, format FROM textures ORDER BY name;";
const char *listMaterialsSQL = "SELECT name, ambient, diffuse, specular, shininess FROM materials ORDER BY name;";
const char *listArmaturesSQL = "SELECT name, vert_count, bone_count FROM armatures ORDER BY name;";
const char *listAnimationsSQL = "SELECT name, channel_count, ticks_per_sec FROM animations ORDER BY name;";
const char *addMeshSQL = "INSERT OR REPLACE INTO meshes(name, vert_count, vertices, normals, colors, uvs, index_count, indices) VALUES (?, ?, ?, ?, ?, ?, ?, ?);";
const char *addTextureSQL = "INSERT OR REPLACE INTO textures(name, width, height, format, data) VALUES (?, ?, ?, ?, ?);";
const char *addMaterialSQL = "INSERT OR REPLACE INTO materials(name, ambient, diffuse, specular, shininess) VALUES (?, ?, ?, ?, ?);";
const char *addArmatureSQL = "INSERT OR REPLACE INTO armatures(name, vert_count, vgroups, vweights, bone_count, bones) VALUES (?, ?, ?, ?, ?, ?);";
const char *addAnimationSQL = "INSERT OR REPLACE INTO animations(name, channel_count, ticks_per_sec) VALUES (?, ?, ?);";
const char *addAnimChannelSQL = "INSERT OR REPLACE INTO anim_channels(anim_id, name, pos_key_count, pos_keys, rot_key_count, rot_keys, scl_key_count, scl_keys) VALUES (?, ?, ?, ?, ?, ?, ?, ?);";


//Globals
//===========================================================================
char cmd[256];
sqlite3 *db = NULL;


//Functions
//===========================================================================
void Quit(void)
{
    //Close any open asset database
    if(db)
    {
        sqlite3_close_v2(db);
    }
}


int Init(void)
{
    //Initialize
    atexit(&Quit);
    return CYB_NO_ERROR;
}


void ConvertMatrix(Cyb_Mat4 *out, struct aiMatrix4x4 *in)
{
    //Transpose the assimp matrix into the Cybermals matrix
    out->a = in->a1;
    out->b = in->a2;
    out->c = in->a3;
    out->d = in->a4;
    
    out->e = in->b1;
    out->f = in->b2;
    out->g = in->b3;
    out->h = in->b4;
    
    out->i = in->c1;
    out->j = in->c2;
    out->k = in->c3;
    out->l = in->c4;
    
    out->m = in->d1;
    out->n = in->d2;
    out->o = in->d3;
    out->p = in->d4;
}


int OpenAssetDB(const char *filename)
{
    //Close open asset database
    if(db)
    {
        sqlite3_close_v2(db);
    }
    
    //Open asset database
    if(sqlite3_open_v2(filename, &db, 
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) != SQLITE_OK)
    {
        return CYB_ERROR;
    }
    
    //Initialize the asset database
    if(sqlite3_exec(db, initSQL, NULL, NULL, NULL) != SQLITE_OK)
    {
        puts(sqlite3_errmsg(db));
        sqlite3_close_v2(db);
        db = NULL;
        return CYB_ERROR;
    }
    
    return CYB_NO_ERROR;
}


int ListAssets(void)
{
    //Make sure there is an open database
    if(!db)
    {
        puts("ERROR: No open asset database.");
        return CYB_ERROR;
    }

    //Compile SQL statements
    sqlite3_stmt *listMeshesStmt = NULL;
    sqlite3_stmt *listTexturesStmt = NULL;
    sqlite3_stmt *listMaterialsStmt = NULL;
    sqlite3_stmt *listArmaturesStmt = NULL;
    sqlite3_stmt *listAnimationsStmt = NULL;
    
    if(sqlite3_prepare_v2(db, listMeshesSQL, -1, &listMeshesStmt, NULL) != 
        SQLITE_OK)
    {
        return CYB_ERROR;
    }
    
    if(sqlite3_prepare_v2(db, listTexturesSQL, -1, &listTexturesStmt, NULL) !=
        SQLITE_OK)
    {
        sqlite3_finalize(listMeshesStmt);
        return CYB_ERROR;
    }
    
    if(sqlite3_prepare_v2(db, listMaterialsSQL, -1, &listMaterialsStmt, NULL) !=
        SQLITE_OK)
    {
        sqlite3_finalize(listMeshesStmt);
        sqlite3_finalize(listTexturesStmt);
        return CYB_ERROR;
    }
    
    if(sqlite3_prepare_v2(db, listArmaturesSQL, -1, &listArmaturesStmt, NULL) !=
        SQLITE_OK)
    {
        sqlite3_finalize(listMeshesStmt);
        sqlite3_finalize(listTexturesStmt);
        sqlite3_finalize(listMaterialsStmt);
        return CYB_ERROR;
    }
    
    if(sqlite3_prepare_v2(db, listAnimationsSQL, -1, &listAnimationsStmt, NULL) !=
        SQLITE_OK)
    {
        sqlite3_finalize(listMeshesStmt);
        sqlite3_finalize(listTexturesStmt);
        sqlite3_finalize(listMaterialsStmt);
        sqlite3_finalize(listArmaturesStmt);
        return CYB_ERROR;
    }
    
    //List all meshes
    puts("Meshes");
    puts("======");
    
    while(sqlite3_step(listMeshesStmt) == SQLITE_ROW)
    {
        //Print current mesh
        printf("Name: %s\n", sqlite3_column_text(listMeshesStmt, 0));
        printf("Vertices: %i\n", sqlite3_column_int(listMeshesStmt, 1));
        printf("Indices: %i\n\n", sqlite3_column_int(listMeshesStmt, 2));
    }
    
    //List all textures
    puts("");
    puts("Textures");
    puts("========");
    
    while(sqlite3_step(listTexturesStmt) == SQLITE_ROW)
    {
        //Print current texture
        printf("Name: %s\n", sqlite3_column_text(listTexturesStmt, 0));
        printf("Width: %i\n", sqlite3_column_int(listTexturesStmt, 1));
        printf("Height: %i\n", sqlite3_column_int(listTexturesStmt, 2));
        printf("Format: %s\n\n", sqlite3_column_text(listTexturesStmt, 3));
    }
    
    //List all materials
    puts("");
    puts("Materials");
    puts("=========");
    
    while(sqlite3_step(listMaterialsStmt) == SQLITE_ROW)
    {
        //Print current material
        Cyb_Vec3 *ambient = (Cyb_Vec3*)sqlite3_column_blob(listMaterialsStmt, 1);
        Cyb_Vec3 *diffuse = (Cyb_Vec3*)sqlite3_column_blob(listMaterialsStmt, 2);
        Cyb_Vec3 *specular = (Cyb_Vec3*)sqlite3_column_blob(listMaterialsStmt, 3);
        printf("Name: %s\n", sqlite3_column_text(listMaterialsStmt, 0));
        printf(
            "Ambient: RGB(%f, %f, %f)\n",
            ambient->x,
            ambient->y,
            ambient->z
        );
        printf(
            "Diffuse: RGB(%f, %f, %f)\n",
            diffuse->x,
            diffuse->y,
            diffuse->z
        );
        printf(
            "Specular: RGB(%f, %f, %f)\n",
            specular->x,
            specular->y,
            specular->z
        );
        printf("Shininess: %f\n\n", sqlite3_column_double(listMaterialsStmt, 4));
    }
    
    //List all armatures
    puts("");
    puts("Armatures");
    puts("=========");
    
    while(sqlite3_step(listArmaturesStmt) == SQLITE_ROW)
    {
        printf("Name: %s\n", sqlite3_column_text(listArmaturesStmt, 0));
        printf("Vertex Count: %i\n", sqlite3_column_int(listArmaturesStmt, 1));
        printf("Bone Count: %i\n\n", sqlite3_column_int(listArmaturesStmt, 2));
    }
    
    //List all animations
    puts("");
    puts("Animations");
    puts("==========");
    
    while(sqlite3_step(listAnimationsStmt) == SQLITE_ROW)
    {
        printf("Name: %s\n", sqlite3_column_text(listAnimationsStmt, 0));
        printf("Channel Count: %i\n", sqlite3_column_int(listAnimationsStmt, 1));
        printf("Ticks Per Second: %d\n\n", 
            sqlite3_column_double(listAnimationsStmt, 2));
    }
    
    //Finalize SQL statements
    sqlite3_finalize(listMeshesStmt);
    sqlite3_finalize(listTexturesStmt);
    sqlite3_finalize(listMaterialsStmt);
    sqlite3_finalize(listArmaturesStmt);
    sqlite3_finalize(listAnimationsStmt);
    return CYB_NO_ERROR;
}


int AddMeshes(const char *filename)
{
    //Make sure there is an open database
    if(!db)
    {
        puts("ERROR: No open asset database.");
        return CYB_ERROR;
    }
    
    //Open mesh file
    printf("Opening mesh file '%s'...\n", filename);
    const struct aiScene *scene = aiImportFile(filename, 
        aiProcess_Triangulate | 
        aiProcess_SortByPType | 
        aiProcess_JoinIdenticalVertices | 
        aiProcess_OptimizeMeshes | 
        aiProcess_CalcTangentSpace | 
        aiProcess_GenBoundingBoxes | 
        aiProcess_FlipUVs | 
        aiProcess_EmbedTextures | 
        aiProcess_PopulateArmatureData | 
        aiProcess_ValidateDataStructure | 
        aiProcess_FindInvalidData);
    
    if(!scene)
    {
        return CYB_ERROR;
    }
    
    //Compile SQL statements
    sqlite3_stmt *addMeshStmt = NULL;
    sqlite3_stmt *addTextureStmt = NULL;
    sqlite3_stmt *addMaterialStmt = NULL;
    sqlite3_stmt *addArmatureStmt = NULL;
    sqlite3_stmt *addAnimationStmt = NULL;
    sqlite3_stmt *addAnimChannelStmt = NULL;
    
    if(sqlite3_prepare_v2(db, addMeshSQL, -1, &addMeshStmt, NULL) != SQLITE_OK)
    {
        return CYB_ERROR;
    }
    
    if(sqlite3_prepare_v2(db, addTextureSQL, -1, &addTextureStmt, NULL) !=
        SQLITE_OK)
    {
        sqlite3_finalize(addMeshStmt);
        return CYB_ERROR;
    }
    
    if(sqlite3_prepare_v2(db, addMaterialSQL, -1, &addMaterialStmt, NULL) !=
        SQLITE_OK)
    {
        sqlite3_finalize(addMeshStmt);
        sqlite3_finalize(addTextureStmt);
        return CYB_ERROR;
    }
    
    if(sqlite3_prepare_v2(db, addArmatureSQL, -1, &addArmatureStmt, NULL) !=
        SQLITE_OK)
    {
        sqlite3_finalize(addMeshStmt);
        sqlite3_finalize(addTextureStmt);
        sqlite3_finalize(addMaterialStmt);
        return CYB_ERROR;
    }
    
    if(sqlite3_prepare_v2(db, addAnimationSQL, -1, &addAnimationStmt, NULL) !=
        SQLITE_OK)
    {
        sqlite3_finalize(addMeshStmt);
        sqlite3_finalize(addTextureStmt);
        sqlite3_finalize(addMaterialStmt);
        sqlite3_finalize(addArmatureStmt);
        return CYB_ERROR;
    }
    
    if(sqlite3_prepare_v2(db, addAnimChannelSQL, -1, &addAnimChannelStmt, NULL) !=
        SQLITE_OK)
    {
        sqlite3_finalize(addMeshStmt);
        sqlite3_finalize(addTextureStmt);
        sqlite3_finalize(addMaterialStmt);
        sqlite3_finalize(addArmatureStmt);
        sqlite3_finalize(addAnimationStmt);
        return CYB_ERROR;
    }
    
    //Add all meshes in the mesh file to the asset database
    printf("Enumerating %i meshes...\n", scene->mNumMeshes);
    
    for(int i = 0; i < scene->mNumMeshes; i++)
    {
        //Get current mesh
        struct aiMesh *mesh = scene->mMeshes[i];
        printf("Adding mesh '%s' to asset database...", mesh->mName.data);
        
        //Convert UVs to 2D vectors
        Cyb_Vec2 *uvs = NULL;
        
        if(mesh->mTextureCoords[0])
        {
            uvs = (Cyb_Vec2*)malloc(
                sizeof(Cyb_Vec2) * mesh->mNumVertices);
        
            if(!uvs)
            {
                puts("failed");
                continue;
            }
        
            for(int i = 0; i < mesh->mNumVertices; i++)
            {
                memcpy(&uvs[i], &mesh->mTextureCoords[0][i], sizeof(Cyb_Vec2));
            }
        }
        
        //Convert faces to indices
        int *indices = (int*)malloc(sizeof(int) * mesh->mNumFaces * 3);
        
        if(!indices)
        {
            puts("failed");
            free(uvs);
            continue;
        }
        
        for(int i = 0; i < mesh->mNumFaces; i++)
        {
            indices[i * 3] = mesh->mFaces[i].mIndices[0];
            indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
            indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
        }
        
        //Add new mesh
        sqlite3_reset(addMeshStmt);
        sqlite3_bind_text(addMeshStmt, 1, mesh->mName.data, -1, NULL);
        sqlite3_bind_int(addMeshStmt, 2, mesh->mNumVertices);
        sqlite3_bind_blob(addMeshStmt, 3, mesh->mVertices, 
            sizeof(Cyb_Vec3) * mesh->mNumVertices, NULL);
            
        if(mesh->mNormals)
        {
            sqlite3_bind_blob(addMeshStmt, 4, mesh->mNormals,
                sizeof(Cyb_Vec3) * mesh->mNumVertices, NULL);
        }
        else
        {
            sqlite3_bind_null(addMeshStmt, 4);
        }
        
        if(mesh->mColors[0])
        {
            sqlite3_bind_blob(addMeshStmt, 5, mesh->mColors[0],
                sizeof(Cyb_Vec4) * mesh->mNumVertices, NULL);
        }
        else
        {
            sqlite3_bind_null(addMeshStmt, 5);
        }
        
        if(uvs)
        {
            sqlite3_bind_blob(addMeshStmt, 6, uvs,
                sizeof(Cyb_Vec2) * mesh->mNumVertices, NULL);
        }
        else
        {
            sqlite3_bind_null(addMeshStmt, 6);
        }
        
        sqlite3_bind_int(addMeshStmt, 7, mesh->mNumFaces * 3);
        sqlite3_bind_blob(addMeshStmt, 8, indices, 
            sizeof(int) * 3 * mesh->mNumFaces, NULL);
        
        if(sqlite3_step(addMeshStmt) != SQLITE_DONE)
        {
            puts("failed");
            
            if(uvs)
            {
                free(uvs);
            }
            
            free(indices);
            continue;
        }
        
        //Free converted UVs and indices
        if(uvs)
        {
            free(uvs);
        }
        
        free(indices);
        puts("ok");
        
        //Does the mesh have bones?
        if(mesh->mNumBones)
        {
            //Process armature
            printf("Adding armature '%s' to the asset database...", 
                mesh->mName.data);
            
            //Initialize vertex group array
            Cyb_Vec4 *vgroups = (Cyb_Vec4*)malloc(
                sizeof(Cyb_Vec4) * mesh->mNumVertices);
                
            if(!vgroups)
            {
                puts("failed");
                continue;
            }
            
            for(int i = 0; i < mesh->mNumVertices; i++)
            {
                vgroups[i].x = -1.0f;
                vgroups[i].y = -1.0f;
                vgroups[i].z = -1.0f;
                vgroups[i].w = -1.0f;
            }
            
            //Initialize vertex weight array
            Cyb_Vec4 *vweights = (Cyb_Vec4*)malloc(
                sizeof(Cyb_Vec4) * mesh->mNumVertices);
                
            if(!vweights)
            {
                puts("failed");
                free(vgroups);
                continue;
            }
            
            //Initialize bone array
            Cyb_Bone *bones = (Cyb_Bone*)malloc(sizeof(Cyb_Bone) * mesh->mNumBones);
            
            if(!bones)
            {
                puts("failed");
                free(vgroups);
                free(vweights);
                continue;
            }
            
            //Process bones
            for(int i = 0; i < mesh->mNumBones; i++)
            {
                //Get next bone
                struct aiBone *bone = mesh->mBones[i];
                
                //Process vertex groups and weights
                for(int j = 0; j < bone->mNumWeights; j++)
                {
                    //Process next group and weight
                    int vertex = bone->mWeights[j].mVertexId;
                    Cyb_Vec4 *vgroup = &vgroups[vertex];
                    Cyb_Vec4 *vweight = &vweights[vertex];
                    
                    if(vgroup->x == -1.0f)
                    {
                        vgroup->x = i;
                        vweight->x = bone->mWeights[j].mWeight;
                    }
                    else if(vgroup->y == -1.0f)
                    {
                        vgroup->y = i;
                        vweight->y = bone->mWeights[j].mWeight;
                    }
                    else if(vgroup->z == -1.0f)
                    {
                        vgroup->z = i;
                        vweight->z = bone->mWeights[j].mWeight;
                    }
                    else if(vgroup->w == -1.0f)
                    {
                        vgroup->w = i;
                        vweight->w = bone->mWeights[j].mWeight;
                    }
                }
                
                //Process bone
                strncpy(bones[i].name, bone->mName.data, 31);
                bones[i].parent = -1;
                struct aiNode *parent = bone->mNode->mParent;
                
                if(parent)
                {
                    for(int j = 0; j < mesh->mNumBones; j++)
                    {
                        //Get next bone
                        struct aiBone *bone2 = mesh->mBones[j];
                        
                        //Found parent bone?
                        if(strcmp(bone2->mName.data, parent->mName.data) == 0)
                        {
                            bones[i].parent = j;
                            break;
                        }
                    }
                }
                
                ConvertMatrix(&bones[i].matrix, &bone->mOffsetMatrix);
            }
            
            //Add the armature to the asset database
            sqlite3_reset(addArmatureStmt);
            sqlite3_bind_text(addArmatureStmt, 1, mesh->mName.data, -1, NULL);
            sqlite3_bind_int(addArmatureStmt, 2, mesh->mNumVertices);
            sqlite3_bind_blob(addArmatureStmt, 3, vgroups, 
                sizeof(Cyb_Vec4) * mesh->mNumVertices, NULL);
            sqlite3_bind_blob(addArmatureStmt, 4, vweights, 
                sizeof(Cyb_Vec4) * mesh->mNumVertices, NULL);
            sqlite3_bind_int(addArmatureStmt, 5, mesh->mNumBones);
            sqlite3_bind_blob(addArmatureStmt, 6, bones, 
                sizeof(Cyb_Bone) * mesh->mNumBones, NULL);
                
            if(sqlite3_step(addArmatureStmt) != SQLITE_DONE)
            {
                puts("failed");
                free(vgroups);
                free(vweights);
                free(bones);
                continue;
            }
            
            //Free vertex group, vertex weight, and bone arrays
            free(vgroups);
            free(vweights);
            free(bones);
            puts("ok");
        }
    }
    
    //Add all textures in the mesh file to the asset database
    for(int i = 0; i < scene->mNumTextures; i++)
    {
        //Get current texture
        struct aiTexture *tex = scene->mTextures[i];
        printf("Adding texture '%s' (format = '%s') to the asset database...", 
            tex->mFilename.data, tex->achFormatHint);
        
        //Texture added
        //puts("ok");
        puts("unsupported");
    }
    
    //Add all materials in the mesh file to the asset database
    for(int i = 0; i < scene->mNumMaterials; i++)
    {
        //Get current material
        struct aiMaterial *mat = scene->mMaterials[i];
        struct aiString name;
        
        if(aiGetMaterialString(mat, AI_MATKEY_NAME, &name) != AI_SUCCESS)
        {
            continue;
        }
        
        printf("Adding material '%s' to the asset database...", name.data);
        
        struct aiColor4D ambient;
        struct aiColor4D diffuse;
        struct aiColor4D specular;
        float shininess;
        int shininessMax = sizeof(shininess);
        
        if(aiGetMaterialColor(mat, AI_MATKEY_COLOR_AMBIENT, &ambient) != AI_SUCCESS)
        {
            puts("failed");
            continue;
        }
        
        if(aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuse) != AI_SUCCESS)
        {
            puts("failed");
            continue;
        }
        
        if(aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &specular) != 
            AI_SUCCESS)
        {
            puts("failed");
            continue;
        }
        
        if(aiGetMaterialFloatArray(mat, AI_MATKEY_SHININESS, &shininess, 
            &shininessMax) != AI_SUCCESS)
        {
            puts("failed");
            continue;
        }
        
        //Add new material
        sqlite3_reset(addMaterialStmt);
        sqlite3_bind_text(addMaterialStmt, 1, name.data, -1, NULL);
        sqlite3_bind_blob(addMaterialStmt, 2, &ambient, sizeof(struct aiColor3D),
            NULL);
        sqlite3_bind_blob(addMaterialStmt, 3, &diffuse, sizeof(struct aiColor3D),
            NULL);
        sqlite3_bind_blob(addMaterialStmt, 4, &specular, sizeof(struct aiColor3D),
            NULL);
        sqlite3_bind_double(addMaterialStmt, 5, shininess);
        
        if(sqlite3_step(addMaterialStmt) != SQLITE_DONE)
        {
            puts("failed");
            continue;
        }
        
        //Material added
        puts("ok");
    }
    
    //Add all animations to the asset database
    for(int i = 0; i < scene->mNumAnimations; i++)
    {
        //Get next animation
        struct aiAnimation *anim = scene->mAnimations[i];
        
        //Add new animation
        printf("Adding animation '%s' to the asset database...", anim->mName.data);
        sqlite3_reset(addAnimationStmt);
        sqlite3_bind_text(addAnimationStmt, 1, anim->mName.data, -1, NULL);
        sqlite3_bind_int(addAnimationStmt, 2, anim->mNumChannels);
        sqlite3_bind_double(addAnimationStmt, 3, anim->mTicksPerSecond);
        
        if(sqlite3_step(addAnimationStmt) != SQLITE_DONE)
        {
            puts("failed");
            continue;
        }
        
        long long animID = sqlite3_last_insert_rowid(db);
        puts("ok");
        
        //Add animation channels
        printf("%s", "Adding animation channels to the asset database...");
        
        for(int j = 0; j < anim->mNumChannels; j++)
        {
            //Get next animation channel
            struct aiNodeAnim *channel = anim->mChannels[j];
            
            //Convert rotation keys to Cyb_Vec4 format
            Cyb_QuatKey *rotKeys = NULL;
            
            if(channel->mRotationKeys)
            {
                rotKeys = (Cyb_QuatKey*)malloc(
                    sizeof(Cyb_QuatKey) * channel->mNumRotationKeys);
                    
                if(!rotKeys)
                {
                    puts("failed");
                    break;
                }
                
                for(int i = 0; i < channel->mNumRotationKeys; i++)
                {
                    rotKeys[i].time = channel->mRotationKeys[i].mTime;
                    rotKeys[i].value.x = channel->mRotationKeys[i].mValue.x;
                    rotKeys[i].value.y = channel->mRotationKeys[i].mValue.y;
                    rotKeys[i].value.z = channel->mRotationKeys[i].mValue.z;
                    rotKeys[i].value.w = channel->mRotationKeys[i].mValue.w;
                }
            }
            
            //Add animation channel
            sqlite3_reset(addAnimChannelStmt);
            sqlite3_bind_int64(addAnimChannelStmt, 1, animID);
            sqlite3_bind_text(addAnimChannelStmt, 2, channel->mNodeName.data, -1, 
                NULL);
            sqlite3_bind_int(addAnimChannelStmt, 3, channel->mNumPositionKeys);
            
            if(channel->mPositionKeys)
            {
                sqlite3_bind_blob(addAnimChannelStmt, 4, channel->mPositionKeys, 
                    sizeof(struct aiVectorKey) * channel->mNumPositionKeys, NULL);
            }
            else
            {
                sqlite3_bind_null(addAnimChannelStmt, 4);
            }
        
            sqlite3_bind_int(addAnimChannelStmt, 5, channel->mNumRotationKeys);
            
            if(rotKeys)
            {
                sqlite3_bind_blob(addAnimChannelStmt, 6, rotKeys, 
                    sizeof(struct aiQuatKey) * channel->mNumRotationKeys, NULL);
            }
            else
            {
                sqlite3_bind_null(addAnimChannelStmt, 6);
            }
            
            sqlite3_bind_int(addAnimChannelStmt, 7, channel->mNumScalingKeys);
            
            if(channel->mScalingKeys)
            {
                sqlite3_bind_blob(addAnimChannelStmt, 8, channel->mScalingKeys, 
                    sizeof(struct aiVectorKey) * channel->mNumScalingKeys, NULL);
            }
            else
            {
                sqlite3_bind_null(addAnimChannelStmt, 8);
            }
                
            if(sqlite3_step(addAnimChannelStmt) != SQLITE_DONE)
            {
                puts("failed");
                
                if(rotKeys)
                {
                    free(rotKeys);
                }
                
                break;
            }
            
            //Free converted rotation keys
            if(rotKeys)
            {
                free(rotKeys);
            }
        }
        
        puts("ok");
    }
    
    //Finalize SQL statements
    sqlite3_finalize(addMeshStmt);
    sqlite3_finalize(addTextureStmt);
    sqlite3_finalize(addMaterialStmt);
    sqlite3_finalize(addArmatureStmt);
    sqlite3_finalize(addAnimationStmt);
    sqlite3_finalize(addAnimChannelStmt);
    
    //Free scene
    aiReleaseImport(scene);
    return CYB_NO_ERROR;
}


int AddTexture(const char *filename, const char *name)
{
    //Make sure there is an open database
    if(!db)
    {
        puts("ERROR: No open asset database.");
        return CYB_ERROR;
    }
    
    //Compile SQL statements
    sqlite3_stmt *addTextureStmt = NULL;
    
    if(sqlite3_prepare_v2(db, addTextureSQL, -1, &addTextureStmt, NULL) != 
        SQLITE_OK)
    {
        puts("Failed to compile an SQL statement.");
        return CYB_ERROR;
    }
    
    //Load the texture file
    FILE *file = fopen(filename, "rb");
    
    if(!file)
    {
        printf("Failed to open texture '%s'.\n", filename);
        sqlite3_finalize(addTextureStmt);
        return CYB_ERROR;
    }
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    unsigned char *data = (unsigned char*)malloc(size);
    
    if(!data)
    {
        puts("Out of Memory");
        fclose(file);
        sqlite3_finalize(addTextureStmt);
        return CYB_ERROR;
    }
    
    if(fread(data, sizeof(unsigned char), size, file) < size)
    {
        puts("IOError");
        free(data);
        fclose(file);
        sqlite3_finalize(addTextureStmt);
        return CYB_ERROR;
    }
    
    fclose(file);
    
    //Add the texture to the asset database
    printf("Adding texture '%s' to the asset database...", name);
    char *format = "unknown";
    
    if(strstr(filename, "png"))
    {
        format = "png";
    }
    else if(strstr(filename, "jpg"))
    {
        format = "jpg";
    }
    
    sqlite3_bind_text(addTextureStmt, 1, name, -1, NULL);
    sqlite3_bind_int(addTextureStmt, 2, -1);
    sqlite3_bind_int(addTextureStmt, 3, -1);
    sqlite3_bind_text(addTextureStmt, 4, format, -1, NULL);
    sqlite3_bind_blob(addTextureStmt, 5, data, size, NULL);
    
    if(sqlite3_step(addTextureStmt) != SQLITE_DONE)
    {
        puts("failed");
        free(data);
        sqlite3_finalize(addTextureStmt);
        return CYB_ERROR;
    }
    
    //Finalize SQL statements
    free(data);
    sqlite3_finalize(addTextureStmt);
    puts("ok");
    return CYB_NO_ERROR;
}


//Entry Point
//===========================================================================
int main(int argc, char **argv)
{
    //Initialize
    if(Init())
    {
        puts("Failed to initialize!");
        return 1;
    }
    
    //Display app header
    printf("%s v%s\n\nType 'help' for a list of commands.\n", APP_TITLE, 
        APP_VERSION);
    
    //Main Loop
    while(TRUE)
    {
        //Get next command
        printf("%s", "cmd> ");
        fgets(cmd, sizeof(cmd), stdin);
        cmd[strlen(cmd) - 1] = 0;
        
        //Help?
        if(strcmp(cmd, "help") == 0)
        {
            //Display command reference
            puts(cmdRef);
        }
        //Quit?
        else if(strcmp(cmd, "quit") == 0)
        {
            return 0;
        }
        //Open?
        else if(strncmp(cmd, "open ", 5) == 0)
        {
            //Open asset database
            if(OpenAssetDB(&cmd[5]))
            {
                printf("Failed to open asset database '%s'\n", &cmd[5]);
            }
            else
            {
                printf("Asset database '%s' opened.\n", &cmd[5]);
            }
        }
        //List assets?
        else if(strcmp(cmd, "list assets") == 0)
        {
            //List the assets in the open database
            if(ListAssets())
            {
                puts("Failed to list assets.");
            }
        }
        //Add mesh?
        else if(strncmp(cmd, "add mesh ", 9) == 0)
        {
            //Add the mesh to the open database
            if(AddMeshes(&cmd[9]))
            {
                puts("Failed to add mesh.");
            }
        }
        //Add texture?
        else if(strncmp(cmd, "add texture ", 12) == 0)
        {
            //Add the texture to the open database
            char filename[256];
            char name[256];
            
            if(sscanf(cmd, "add texture %255s %255s", filename, name) < 2)
            {
                puts("Failed to parse command params.");
                continue;
            }
            
            if(AddTexture(filename, name))
            {
                puts("Failed to add texture.");
            }
        }
        //Invalid command?
        else
        {
            puts("Invalid Command");
        }
    }

    return 0;
}