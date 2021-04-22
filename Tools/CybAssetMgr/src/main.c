/*
Cybermals Engine - Asset Manager Tool
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sqlite3.h>

#include "CybCommon.h"

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
"list assets - list the assets in the open database";

const char *initSQL = "PRAGMA foreign_keys=ON;\n"
"\n"
"BEGIN TRANSACTION;\n"
"    CREATE TABLE IF NOT EXISTS assets(\n"
"        name varchar(64) PRIMARY KEY,\n"
"        type varchar(16),\n"
"        version REAL\n"
"    );\n"
"\n"
"    INSERT OR IGNORE INTO assets VALUES ('index', 'ASSET_INDEX', 1.0);\n"
"\n"
"    CREATE TABLE IF NOT EXISTS meshes(\n"
"        name varchar(64),\n"
"        vert_count INT,\n"
"        vertices BLOB,\n"
"        normals BLOB,\n"
"        colors BLOB,\n"
"        uvs BLOB,\n"
"        index_count INT,\n"
"        indices BLOB,\n"
"        CONSTRAINT mesh_name FOREIGN KEY(name) REFERENCES assets(name) ON DELETE CASCADE\n"
"    );\n"
"END TRANSACTION;";

const char *listAssetsSQL = "SELECT * FROM assets;";


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
    //Compile query
    sqlite3_stmt *query;
    
    if(sqlite3_prepare_v2(db, listAssetsSQL, -1, &query, NULL) != SQLITE_OK)
    {
        return CYB_ERROR;
    }
    
    //Execute query
    puts("Assets\n======");
    
    while(sqlite3_step(query) == SQLITE_ROW)
    {
        //Print current asset
        printf("%s (%s)\n", sqlite3_column_text(query, 0), 
            sqlite3_column_text(query, 1));
    }
    
    //Finalize query
    sqlite3_finalize(query);
    return CYB_NO_ERROR;
}


int AddMesh(const char *filename)
{
    //Open mesh file
    //<========= mesh loading code here
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
            if(AddMesh(&cmd[9]))
            {
                puts("Failed to add mesh.");
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