/*
CybRender - Mesh API
*/

#include <string.h>

#include "CybMesh.h"
#include "CybObject.h"


//Structures
//=================================================================================
struct Cyb_Mesh
{
    Cyb_Object base;
    Cyb_Renderer *renderer;
    int vFormat;
    int indexCount;
    GLuint vbo;
    GLuint ebo;
};


//Functions
//=================================================================================
void Cyb_FreeMesh(Cyb_Mesh *mesh)
{
    //Unbind VBO and EBO
    Cyb_SelectRenderer(mesh->renderer);
    Cyb_GLExtAPI *glExtAPI = Cyb_GetGLExtAPI(mesh->renderer);
    glExtAPI->BindBuffer(GL_ARRAY_BUFFER, 0);
    glExtAPI->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    //Free VBO
    if(mesh->vbo)
    {
        glExtAPI->DeleteBuffers(1, &mesh->vbo);
    }
    
    //Free EBO
    if(mesh->ebo)
    {
        glExtAPI->DeleteBuffers(1, &mesh->ebo);
    }
}


Cyb_Mesh *Cyb_CreateMesh(Cyb_Renderer *renderer)
{
    //Allocate new mesh
    Cyb_Mesh *mesh = (Cyb_Mesh*)Cyb_CreateObject(sizeof(Cyb_Mesh),
        (Cyb_FreeProc)&Cyb_FreeMesh, CYB_MESH);
        
    if(!mesh)
    {
        return NULL;
    }
    
    //Initialize the mesh
    mesh->vFormat = CYB_VERTEX_UNKNOWN;
    mesh->renderer = renderer;
    Cyb_SelectRenderer(renderer);
    Cyb_GLExtAPI *glExtAPI = Cyb_GetGLExtAPI(renderer);
    glExtAPI->GenBuffers(1, &mesh->vbo);
    
    if(!mesh->vbo)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "[CybRender] Failed to allocate VBO.");
        Cyb_FreeObject((Cyb_Object**)&mesh);
        return NULL;
    }

    glExtAPI->GenBuffers(1, &mesh->ebo);
    
    if(!mesh->ebo)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s",
            "[CybRender] Failed to allocate EBO.");
        Cyb_FreeObject((Cyb_Object**)&mesh);
        return NULL;
    }

    return mesh;
}


int Cyb_UpdateMesh(Cyb_Renderer *renderer, Cyb_Mesh *mesh, int vertCount, 
    const Cyb_Vec3 *verts, const Cyb_Vec3 *norms, const Cyb_Vec4 *colors, 
    const Cyb_Vec2 *uvs, int indexCount, const unsigned int *indices)
{
    //Ensure that vertices and indices were given
    if(!verts || !indices)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s",
            "[CybRender] Function 'Cyb_UpdateMesh' requires at least an array of vertices and indices.");
        return CYB_ERROR;
    }
    
    //Select the renderer
    Cyb_SelectRenderer(renderer);
    Cyb_GLExtAPI *glExtAPI = Cyb_GetGLExtAPI(renderer);
    
    //Choose the vertex data format
    if(norms)
    {
        if(!colors && !uvs)
        {
            //Bind and map VBO
            glExtAPI->BindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
            glExtAPI->BufferData(GL_ARRAY_BUFFER, sizeof(Cyb_VertexVN) * vertCount,
                NULL, GL_STATIC_DRAW);
            Cyb_VertexVN *buf = glExtAPI->MapBufferRange(GL_ARRAY_BUFFER, 0, 
                sizeof(Cyb_VertexVN) * vertCount, GL_MAP_WRITE_BIT);
        
            if(!buf)
            {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s",
                    "[CybRender] Failed to map VBO.");
                return CYB_ERROR;
            }
        
            //Copy the vertex data
            for(int i = 0; i < vertCount; i++)
            {
                memcpy(&buf[i].pos, &verts[i], sizeof(verts[i]));
                memcpy(&buf[i].norm, &norms[i], sizeof(norms[i]));
            }
        
            //Unmap VBO
            glExtAPI->UnmapBuffer(GL_ARRAY_BUFFER);
            
            //Set vertex format
            mesh->vFormat = CYB_VERTEX_VN;
        }
        else if(colors && !uvs)
        {
            //Bind and map VBO
            glExtAPI->BindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
            glExtAPI->BufferData(GL_ARRAY_BUFFER, sizeof(Cyb_VertexVNC) * vertCount,
                NULL, GL_STATIC_DRAW);
            Cyb_VertexVNC *buf = glExtAPI->MapBufferRange(GL_ARRAY_BUFFER, 0,
                sizeof(Cyb_VertexVNC) * vertCount, GL_MAP_WRITE_BIT);
        
            if(!buf)
            {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s",
                    "[CybRender] Failed to map VBO.");
                return CYB_ERROR;
            }
        
            //Copy the vertex data
            for(int i = 0; i < vertCount; i++)
            {
                memcpy(&buf[i].pos, &verts[i], sizeof(verts[i]));
                memcpy(&buf[i].norm, &norms[i], sizeof(norms[i]));
                memcpy(&buf[i].color, &colors[i], sizeof(colors[i]));
            }
        
            //Unmap VBO
            glExtAPI->UnmapBuffer(GL_ARRAY_BUFFER);
            
            //Set vertex format
            mesh->vFormat = CYB_VERTEX_VNC;
        }
        else if(uvs && !colors)
        {
            //Bind and map VBO
            glExtAPI->BindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
            glExtAPI->BufferData(GL_ARRAY_BUFFER, sizeof(Cyb_VertexVNT) * vertCount,
                NULL, GL_STATIC_DRAW);
            Cyb_VertexVNT *buf = glExtAPI->MapBufferRange(GL_ARRAY_BUFFER, 0,
                sizeof(Cyb_VertexVNT) * vertCount, GL_MAP_WRITE_BIT);
        
            if(!buf)
            {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s",
                    "[CybRender] Failed to map VBO.");
                return CYB_ERROR;
            }
        
            //Copy the vertex data
            for(int i = 0; i < vertCount; i++)
            {
                memcpy(&buf[i].pos, &verts[i], sizeof(verts[i]));
                memcpy(&buf[i].norm, &norms[i], sizeof(norms[i]));
                memcpy(&buf[i].uv, &uvs[i], sizeof(uvs[i]));
            }
        
            //Unmap VBO
            glExtAPI->UnmapBuffer(GL_ARRAY_BUFFER);
            
            //Set vertex format
            mesh->vFormat = CYB_VERTEX_VNT;
        }
        else if(colors && uvs)
        {
            //Bind and map VBO
            glExtAPI->BindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
            glExtAPI->BufferData(GL_ARRAY_BUFFER, 
                sizeof(Cyb_VertexVNCT) * vertCount, NULL, GL_STATIC_DRAW);
            Cyb_VertexVNCT *buf = glExtAPI->MapBufferRange(GL_ARRAY_BUFFER, 0,
                sizeof(Cyb_VertexVNCT) * vertCount, GL_MAP_WRITE_BIT);
        
            if(!buf)
            {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s",
                    "[CybRender] Failed to map VBO.");
                return CYB_ERROR;
            }
        
            //Copy the vertex data
            for(int i = 0; i < vertCount; i++)
            {
                memcpy(&buf[i].pos, &verts[i], sizeof(verts[i]));
                memcpy(&buf[i].norm, &norms[i], sizeof(norms[i]));
                memcpy(&buf[i].color, &colors[i], sizeof(colors[i]));
                memcpy(&buf[i].uv, &uvs[i], sizeof(uvs[i]));
            }
        
            //Unmap VBO
            glExtAPI->UnmapBuffer(GL_ARRAY_BUFFER);
            
            //Set vertex format
            mesh->vFormat = CYB_VERTEX_VNCT;
        }
    }
    else
    {
        //Bind and map VBO
        glExtAPI->BindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
        glExtAPI->BufferData(GL_ARRAY_BUFFER, sizeof(Cyb_VertexV) * vertCount,
            NULL, GL_STATIC_DRAW);
        Cyb_VertexV *buf = glExtAPI->MapBufferRange(GL_ARRAY_BUFFER, 0,
            sizeof(Cyb_VertexV) * vertCount, GL_MAP_WRITE_BIT);
        
        if(!buf)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s",
                "[CybRender] Failed to map VBO.");
            return CYB_ERROR;
        }
        
        //Copy the vertex data
        for(int i = 0; i < vertCount; i++)
        {
            memcpy(&buf[i].pos, &verts[i], sizeof(verts[i]));
        }
        
        //Unmap VBO
        glExtAPI->UnmapBuffer(GL_ARRAY_BUFFER);
        
        //Set vertex format
        mesh->vFormat = CYB_VERTEX_V;
    }
    
    //Bind and fill EBO
    glExtAPI->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    glExtAPI->BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCount,
        indices, GL_STATIC_DRAW);
        
    //Set index count
    mesh->indexCount = indexCount;
    return CYB_NO_ERROR;
}


void Cyb_DrawMesh(Cyb_Renderer *renderer, Cyb_Mesh *mesh)
{
    //Select renderer
    Cyb_SelectRenderer(renderer);
    Cyb_GLExtAPI *glExtAPI = Cyb_GetGLExtAPI(renderer);
    
    //Bind VBO and EBO
    glExtAPI->BindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glExtAPI->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    
    //Setup vertex attrib pointers
    switch(mesh->vFormat)
    {
        //Position only
    case CYB_VERTEX_V:
        glExtAPI->EnableVertexAttribArray(0);
        glExtAPI->DisableVertexAttribArray(1);
        glExtAPI->DisableVertexAttribArray(2);
        glExtAPI->DisableVertexAttribArray(3);
        glExtAPI->VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
            sizeof(Cyb_VertexV), (void*)offsetof(Cyb_VertexV, pos));
        break;
        
        //Position and normal
    case CYB_VERTEX_VN:
        glExtAPI->EnableVertexAttribArray(0);
        glExtAPI->EnableVertexAttribArray(1);
        glExtAPI->DisableVertexAttribArray(2);
        glExtAPI->DisableVertexAttribArray(3);
        glExtAPI->VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
            sizeof(Cyb_VertexVN), (void*)offsetof(Cyb_VertexVN, pos));
        glExtAPI->VertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
            sizeof(Cyb_VertexVN), (void*)offsetof(Cyb_VertexVN, norm));
        break;
        
        //Position, normal, and color
    case CYB_VERTEX_VNC:
        glExtAPI->EnableVertexAttribArray(0);
        glExtAPI->EnableVertexAttribArray(1);
        glExtAPI->EnableVertexAttribArray(2);
        glExtAPI->DisableVertexAttribArray(3);
        glExtAPI->VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
            sizeof(Cyb_VertexVNC), (void*)offsetof(Cyb_VertexVNC, pos));
        glExtAPI->VertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
            sizeof(Cyb_VertexVNC), (void*)offsetof(Cyb_VertexVNC, norm));
        glExtAPI->VertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE,
            sizeof(Cyb_VertexVNC), (void*)offsetof(Cyb_VertexVNC, color));
        break;
        
        //Position, normal, and texcoord
    case CYB_VERTEX_VNT:
        glExtAPI->EnableVertexAttribArray(0);
        glExtAPI->EnableVertexAttribArray(1);
        glExtAPI->EnableVertexAttribArray(2);
        glExtAPI->DisableVertexAttribArray(3);
        glExtAPI->VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
            sizeof(Cyb_VertexVNT), (void*)offsetof(Cyb_VertexVNT, pos));
        glExtAPI->VertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
            sizeof(Cyb_VertexVNT), (void*)offsetof(Cyb_VertexVNT, norm));
        glExtAPI->VertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
            sizeof(Cyb_VertexVNT), (void*)offsetof(Cyb_VertexVNT, uv));
        break;
        
        //Position, normal, color, and texcoord
    case CYB_VERTEX_VNCT:
        glExtAPI->EnableVertexAttribArray(0);
        glExtAPI->EnableVertexAttribArray(1);
        glExtAPI->EnableVertexAttribArray(2);
        glExtAPI->EnableVertexAttribArray(3);
        glExtAPI->VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
            sizeof(Cyb_VertexVNCT), (void*)offsetof(Cyb_VertexVNCT, pos));
        glExtAPI->VertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
            sizeof(Cyb_VertexVNCT), (void*)offsetof(Cyb_VertexVNCT, norm));
        glExtAPI->VertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE,
            sizeof(Cyb_VertexVNCT), (void*)offsetof(Cyb_VertexVNCT, color));
        glExtAPI->VertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE,
            sizeof(Cyb_VertexVNCT), (void*)offsetof(Cyb_VertexVNCT, uv));
        break;
    }
    
    //Draw the mesh
    glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, NULL);
}