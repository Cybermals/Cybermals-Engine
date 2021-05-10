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
            //Assemble geometry data
            Cyb_VertexVN *buf = (Cyb_VertexVN*)SDL_malloc(
                sizeof(Cyb_VertexVN) * vertCount);
            
            if(!buf)
            {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s",
                    "[CybRender] Out of Memory");
                return CYB_ERROR;
            }
        
            for(int i = 0; i < vertCount; i++)
            {
                memcpy(&buf[i].pos, &verts[i], sizeof(verts[i]));
                memcpy(&buf[i].norm, &norms[i], sizeof(norms[i]));
            }
        
            //Bind VBO, upload geometry data, and free temp buffer
            glExtAPI->BindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
            glExtAPI->BufferData(GL_ARRAY_BUFFER, sizeof(Cyb_VertexVN) * vertCount,
                buf, GL_STATIC_DRAW);
            SDL_free(buf);
        
            //Set vertex format
            mesh->vFormat = CYB_VERTEX_VN;
        }
        else if(colors && !uvs)
        {
            //Assemble geometry data
            Cyb_VertexVNC *buf = (Cyb_VertexVNC*)SDL_malloc(
                sizeof(Cyb_VertexVNC) * vertCount);
            
            if(!buf)
            {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s",
                    "[CybRender] Out of Memory");
                return CYB_ERROR;
            }
        
            for(int i = 0; i < vertCount; i++)
            {
                memcpy(&buf[i].pos, &verts[i], sizeof(verts[i]));
                memcpy(&buf[i].norm, &norms[i], sizeof(norms[i]));
                memcpy(&buf[i].color, &colors[i], sizeof(colors[i]));
            }
        
            //Bind VBO, upload geometry data, and free temp buffer
            glExtAPI->BindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
            glExtAPI->BufferData(GL_ARRAY_BUFFER, sizeof(Cyb_VertexVNC) * vertCount,
                buf, GL_STATIC_DRAW);
            SDL_free(buf);
        
            //Set vertex format
            mesh->vFormat = CYB_VERTEX_VNC;
        }
        else if(uvs && !colors)
        {
            //Assemble geometry data
            Cyb_VertexVNT *buf = (Cyb_VertexVNT*)SDL_malloc(
                sizeof(Cyb_VertexVNT) * vertCount);
            
            if(!buf)
            {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s",
                    "[CybRender] Out of Memory");
                return CYB_ERROR;
            }
        
            for(int i = 0; i < vertCount; i++)
            {
                memcpy(&buf[i].pos, &verts[i], sizeof(verts[i]));
                memcpy(&buf[i].norm, &norms[i], sizeof(norms[i]));
                memcpy(&buf[i].uv, &uvs[i], sizeof(uvs[i]));
            }
        
            //Bind VBO, upload geometry data, and free temp buffer
            glExtAPI->BindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
            glExtAPI->BufferData(GL_ARRAY_BUFFER, sizeof(Cyb_VertexVNT) * vertCount,
                buf, GL_STATIC_DRAW);
            SDL_free(buf);
        
            //Set vertex format
            mesh->vFormat = CYB_VERTEX_VNT;
        }
        else if(colors && uvs)
        {
            //Assemble geometry data
            Cyb_VertexVNCT *buf = (Cyb_VertexVNCT*)SDL_malloc(
                sizeof(Cyb_VertexVNCT) * vertCount);
            
            if(!buf)
            {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s",
                    "[CybRender] Out of Memory");
                return CYB_ERROR;
            }
        
            for(int i = 0; i < vertCount; i++)
            {
                memcpy(&buf[i].pos, &verts[i], sizeof(verts[i]));
                memcpy(&buf[i].norm, &norms[i], sizeof(norms[i]));
                memcpy(&buf[i].color, &colors[i], sizeof(colors[i]));
                memcpy(&buf[i].uv, &uvs[i], sizeof(uvs[i]));
            }
        
            //Bind VBO, upload geometry data, and free temp buffer
            glExtAPI->BindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
            glExtAPI->BufferData(GL_ARRAY_BUFFER, 
                sizeof(Cyb_VertexVNCT) * vertCount, buf, GL_STATIC_DRAW);
            SDL_free(buf);
        
            //Set vertex format
            mesh->vFormat = CYB_VERTEX_VNCT;
        }
    }
    else
    {
        //Assemble geometry data
        Cyb_VertexV *buf = (Cyb_VertexV*)SDL_malloc(
            sizeof(Cyb_VertexV) * vertCount);
            
        if(!buf)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s",
                "[CybRender] Out of Memory");
            return CYB_ERROR;
        }
        
        for(int i = 0; i < vertCount; i++)
        {
            memcpy(&buf[i].pos, &verts[i], sizeof(verts[i]));
        }
        
        //Bind VBO, upload geometry data, and free temp buffer
        glExtAPI->BindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
        glExtAPI->BufferData(GL_ARRAY_BUFFER, sizeof(Cyb_VertexV) * vertCount,
            buf, GL_STATIC_DRAW);
        SDL_free(buf);
        
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
        glExtAPI->EnableVertexAttribArray(CYB_ATTRIB_POS);
        glExtAPI->DisableVertexAttribArray(CYB_ATTRIB_NORM);
        glExtAPI->DisableVertexAttribArray(CYB_ATTRIB_COLOR);
        glExtAPI->DisableVertexAttribArray(CYB_ATTRIB_UV);
        glExtAPI->VertexAttribPointer(CYB_ATTRIB_POS, 3, GL_FLOAT, GL_FALSE, 
            sizeof(Cyb_VertexV), (void*)offsetof(Cyb_VertexV, pos));
        break;
        
        //Position and normal
    case CYB_VERTEX_VN:
        glExtAPI->EnableVertexAttribArray(CYB_ATTRIB_POS);
        glExtAPI->EnableVertexAttribArray(CYB_ATTRIB_NORM);
        glExtAPI->DisableVertexAttribArray(CYB_ATTRIB_COLOR);
        glExtAPI->DisableVertexAttribArray(CYB_ATTRIB_UV);
        glExtAPI->VertexAttribPointer(CYB_ATTRIB_POS, 3, GL_FLOAT, GL_FALSE, 
            sizeof(Cyb_VertexVN), (void*)offsetof(Cyb_VertexVN, pos));
        glExtAPI->VertexAttribPointer(CYB_ATTRIB_NORM, 3, GL_FLOAT, GL_FALSE,
            sizeof(Cyb_VertexVN), (void*)offsetof(Cyb_VertexVN, norm));
        break;
        
        //Position, normal, and color
    case CYB_VERTEX_VNC:
        glExtAPI->EnableVertexAttribArray(CYB_ATTRIB_POS);
        glExtAPI->EnableVertexAttribArray(CYB_ATTRIB_NORM);
        glExtAPI->EnableVertexAttribArray(CYB_ATTRIB_COLOR);
        glExtAPI->DisableVertexAttribArray(CYB_ATTRIB_UV);
        glExtAPI->VertexAttribPointer(CYB_ATTRIB_POS, 3, GL_FLOAT, GL_FALSE, 
            sizeof(Cyb_VertexVNC), (void*)offsetof(Cyb_VertexVNC, pos));
        glExtAPI->VertexAttribPointer(CYB_ATTRIB_NORM, 3, GL_FLOAT, GL_FALSE,
            sizeof(Cyb_VertexVNC), (void*)offsetof(Cyb_VertexVNC, norm));
        glExtAPI->VertexAttribPointer(CYB_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE,
            sizeof(Cyb_VertexVNC), (void*)offsetof(Cyb_VertexVNC, color));
        break;
        
        //Position, normal, and texcoord
    case CYB_VERTEX_VNT:
        glExtAPI->EnableVertexAttribArray(CYB_ATTRIB_POS);
        glExtAPI->EnableVertexAttribArray(CYB_ATTRIB_NORM);
        glExtAPI->DisableVertexAttribArray(CYB_ATTRIB_COLOR);
        glExtAPI->EnableVertexAttribArray(CYB_ATTRIB_UV);
        glExtAPI->VertexAttribPointer(CYB_ATTRIB_POS, 3, GL_FLOAT, GL_FALSE, 
            sizeof(Cyb_VertexVNT), (void*)offsetof(Cyb_VertexVNT, pos));
        glExtAPI->VertexAttribPointer(CYB_ATTRIB_NORM, 3, GL_FLOAT, GL_FALSE,
            sizeof(Cyb_VertexVNT), (void*)offsetof(Cyb_VertexVNT, norm));
        glExtAPI->VertexAttribPointer(CYB_ATTRIB_UV, 2, GL_FLOAT, GL_FALSE,
            sizeof(Cyb_VertexVNT), (void*)offsetof(Cyb_VertexVNT, uv));
        break;
        
        //Position, normal, color, and texcoord
    case CYB_VERTEX_VNCT:
        glExtAPI->EnableVertexAttribArray(CYB_ATTRIB_POS);
        glExtAPI->EnableVertexAttribArray(CYB_ATTRIB_NORM);
        glExtAPI->EnableVertexAttribArray(CYB_ATTRIB_COLOR);
        glExtAPI->EnableVertexAttribArray(CYB_ATTRIB_UV);
        glExtAPI->VertexAttribPointer(CYB_ATTRIB_POS, 3, GL_FLOAT, GL_FALSE, 
            sizeof(Cyb_VertexVNCT), (void*)offsetof(Cyb_VertexVNCT, pos));
        glExtAPI->VertexAttribPointer(CYB_ATTRIB_NORM, 3, GL_FLOAT, GL_FALSE,
            sizeof(Cyb_VertexVNCT), (void*)offsetof(Cyb_VertexVNCT, norm));
        glExtAPI->VertexAttribPointer(CYB_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE,
            sizeof(Cyb_VertexVNCT), (void*)offsetof(Cyb_VertexVNCT, color));
        glExtAPI->VertexAttribPointer(CYB_ATTRIB_UV, 2, GL_FLOAT, GL_FALSE,
            sizeof(Cyb_VertexVNCT), (void*)offsetof(Cyb_VertexVNCT, uv));
        break;
    }
    
    //Draw the mesh
    glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, NULL);
}