/*
CybUI - Label API
*/

#include "CybLabel.h"


//Functions
//=================================================================================
void Cyb_RenderText(SDL_Renderer *renderer, TTF_Font *font, 
    const SDL_Point *pos, SDL_Color color, const char *text, int flags)
{
    //Validate the params
    if(!font)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "[CybUI] Can't render text without a font!");
        return;
    }
    
    if(!text)
    {
        return;
    }
    
    //Render the text
    SDL_Surface *image = TTF_RenderUTF8_Solid(font, text, color);
    
    if(!image)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[CybUI] %s", SDL_GetError());
        return;
    }
    
    //Create a texture from the text image
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, image);
    
    if(!tex)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[CybUI] %s", SDL_GetError());
        SDL_FreeSurface(image);
        return;
    }
    
    //Calculate text rect
    SDL_Rect rect;
    rect.x = pos->x;
    rect.y = pos->y;
    rect.w = image->w;
    rect.h = image->h;
    
    if(flags & CYB_TEXT_HALIGN_CENTER)
    {
        rect.x -= image->w / 2;
    }
    else if(flags & CYB_TEXT_HALIGN_RIGHT)
    {
        rect.x -= image->w;
    }

    if(flags & CYB_TEXT_VALIGN_CENTER)
    {
        rect.y -= image->h / 2;
    }
    else if(flags & CYB_TEXT_VALIGN_BOTTOM)
    {
        rect.y -= image->h;
    }
    
    //Copy the texture to the UI
    SDL_RenderCopy(renderer, tex, NULL, &rect);
    
    //Free the surface and texture
    SDL_FreeSurface(image);
    SDL_DestroyTexture(tex);
}


void Cyb_DrawLabelProc(Cyb_Grid *grid, SDL_Renderer *renderer)
{
    //Call base draw procedure
    Cyb_DrawGridProc(grid, renderer);
    
    //Draw label text centered
    SDL_Point pos;
    pos.x = grid->viewport.w / 2;
    pos.y = grid->viewport.h / 2;
    Cyb_RenderText(renderer, grid->font, &pos, grid->fg, grid->text,
        CYB_TEXT_HALIGN_CENTER | CYB_TEXT_VALIGN_CENTER);
}


Cyb_Grid *Cyb_CreateLabel(void)
{
    //Allocate new label
    Cyb_Grid *label = Cyb_CreateGrid();
    
    if(!label)
    {
        return NULL;
    }
    
    //Initialize the label
    label->draw = &Cyb_DrawLabelProc;
    return label;
}