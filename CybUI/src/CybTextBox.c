/*
CybUI - TextBox API
*/

#include <SDL2/SDL_ttf.h>

#include "CybLabel.h"
#include "CybList.h"
#include "CybObject.h"
#include "CybTextBox.h"
#include "CybVector.h"


//Structures
//=================================================================================
typedef struct
{
    Cyb_ListNode base;
    int caretPos;
    Cyb_Vector *line;
} Cyb_LineNode;


typedef struct
{
    Cyb_Object base;
    int mode;
    int caretTmr;
    int caretVisible;
    SDL_Point scrollPos;
    int isScrolling;
    int activeLine;
    Cyb_List *lines;
} Cyb_TextBoxData;


//Globals
//=================================================================================
static char lineBuf[1024];


//Functions
//=================================================================================
void Cyb_FreeLineNode(Cyb_LineNode *node)
{
    //Free the line vector
    Cyb_FreeObject((Cyb_Object**)&node->line);
}


void Cyb_FreeTextBoxData(Cyb_TextBoxData *obj)
{
    //Free the lines list
    Cyb_FreeObject((Cyb_Object**)&obj->lines);
}


void Cyb_DrawTextBoxProc(Cyb_Grid *textBox, SDL_Renderer *renderer)
{
    //Call the base draw procedure
    Cyb_DrawGridProc(textBox, renderer);
    
    //Draw each line of text starting with the first visible line
    Cyb_TextBoxData *data = (Cyb_TextBoxData*)textBox->data;
    SDL_Point pos;
    pos.x = -data->scrollPos.x;
    pos.y = -data->scrollPos.y;
    int line = 0;
    int lineInc = TTF_FontLineSkip(textBox->font);
    
    for(Cyb_LineNode *node = (Cyb_LineNode*)data->lines->first; node;
        node = (Cyb_LineNode*)node->base.next)
    {
        //Is this line out of bounds?
        if(pos.y + lineInc < 0 || pos.y > textBox->viewport.h)
        {
            //Increment Y coordinate and line
            pos.y += lineInc;
            line++;
            continue;
        }
        
        //Draw the line of text
        Cyb_RenderText(renderer, textBox->font, &pos, textBox->fg, node->line->data,
            0);
            
        //Draw caret?
        if(Cyb_GetActiveGrid() == textBox && data->caretVisible && 
            line == data->activeLine)
        {
            //Calculate caret X coordinate
            int x = pos.x;
            
            for(int i = 0; i < node->caretPos; i++)
            {
                //Increment X coordinate by char width
                int inc;
                
                if(TTF_GlyphMetrics(textBox->font, ((char*)node->line->data)[i],
                    NULL, NULL, NULL, NULL, &inc) == 0)
                {
                    x += inc;
                }
            }
            
            //Draw caret
            SDL_SetRenderDrawColor(renderer, textBox->fg.r, textBox->fg.g, 
                textBox->fg.b, textBox->fg.a);
            SDL_RenderDrawLine(renderer, x, pos.y, x, 
                pos.y + TTF_FontHeight(textBox->font));
        }
        
        //Increment Y coordinate and line
        pos.y += lineInc;
        line++;
    }
    
    //Update caret state
    if(data->caretTmr-- == 0)
    {
        data->caretVisible = !data->caretVisible;
        data->caretTmr = 30;
    }
}


void Cyb_HandleTextBoxEventProc(Cyb_Grid *textBox, const SDL_Event *event)
{
    //Call the base event handler procedure
    Cyb_HandleGridEventProc(textBox, event);
    
    //Handle the event
    Cyb_TextBoxData *data = (Cyb_TextBoxData*)textBox->data;
    
    switch(event->type)
    {
        //Mouse Motion Event
    case SDL_MOUSEMOTION:
    {
        //Update scrolling
        if(data->isScrolling)
        {
            data->scrollPos.x -= event->motion.xrel;
            data->scrollPos.y -= event->motion.yrel;
            
            if(data->scrollPos.x < 0)
            {
                data->scrollPos.x = 0;
            }
            
            if(data->scrollPos.y < 0)
            {
                data->scrollPos.y = 0;
            }
        }
    
        break;
    }
    
        //Mouse Button Down Event
    case SDL_MOUSEBUTTONDOWN:
    {
        //Ignore this if the mouse was outside the widget
        SDL_Point mousePos;
        mousePos.x = event->button.x;
        mousePos.y = event->button.y;
        
        if(!SDL_PointInRect(&mousePos, &textBox->viewport))
        {
            return;
        }
        
        //Turn on scrolling and text input mode
        data->isScrolling = TRUE;
        SDL_SetTextInputRect(&textBox->viewport);
        SDL_StartTextInput();
        
        //Convert mouse pos to local coords
        SDL_Point localPos;
        Cyb_GlobalToLocal(textBox, &mousePos, &localPos);
        
        //Calculate active line
        int line = (localPos.y + data->scrollPos.y) / 
            TTF_FontLineSkip(textBox->font);
            
        //Calculate column
        Cyb_LineNode *node = (Cyb_LineNode*)Cyb_GetListElm(data->lines, line);
        
        if(!node)
        {
            return;
        }
    
        int x = 0;
        int xprev = x;
        
        for(int i = 0; i < node->line->len; i++)
        {
            //Found the column?
            if(x == localPos.x + data->scrollPos.x)
            {
                Cyb_SetCaretPos(textBox, line, i);
                return;
            }
            else if(x >= localPos.x + data->scrollPos.x)
            {
                Cyb_SetCaretPos(textBox, line, 
                    (x - (localPos.x + data->scrollPos.x) < (localPos.x + data->scrollPos.x) - xprev ? i : i - 1));
                return;
            }
            
            //Update X coordinate
            xprev = x;
            int advance;
            
            if(TTF_GlyphMetrics(textBox->font, ((char*)node->line->data)[i], NULL, 
                NULL, NULL, NULL, &advance) == 0)
            {
                x += advance;
            }
        }
        
        //Move caret to end of line
        Cyb_SetCaretPos(textBox, line, node->line->len - 1);
        break;
    }
    
        //Mouse Button Up Event
    case SDL_MOUSEBUTTONUP:
    {
        //Turn off scrolling
        data->isScrolling = FALSE;
        break;
    }
    
        //Key Down Event
    case SDL_KEYDOWN:
    {
        //Ignore this unless the widget has focus and it isn't read-only
        if(Cyb_GetActiveGrid() != textBox || data->mode & CYB_TEXTBOX_READONLY)
        {
            return;
        }
        
        //Handle special keys
        switch(event->key.keysym.sym)
        {
            //Backspace
        case SDLK_BACKSPACE:
        {
            //Get the active line
            Cyb_LineNode *node = (Cyb_LineNode*)Cyb_GetListElm(data->lines,
                data->activeLine);
                
            if(!node)
            {
                return;
            }
            
            //Is the caret at the beginning?
            if(node->caretPos == 0)
            {
                //Ignore this if the active line is the first
                if(data->activeLine == 0)
                {
                    return;
                }
                
                //Add the active line to the end of the previous line and remove the
                //active line
                int lineLen = node->line->len - 1;
                Cyb_InsertText(textBox, data->activeLine - 1, CYB_VEC_END, 
                    (char*)node->line->data);
                Cyb_RemoveLine(textBox, data->activeLine);
            
                node = (Cyb_LineNode*)Cyb_GetListElm(data->lines, 
                    --data->activeLine);
                
                if(!node)
                {
                    return;
                }
                
                node->caretPos = node->line->len - lineLen - 1;
            }
            //Is the caret elsewhere?
            else
            {
                Cyb_RemoveText(textBox, data->activeLine, --node->caretPos, 1);
                
                if(node->caretPos < 0)
                {
                    node->caretPos = 0;
                }
            }
            
            break;
        }
        
            //Delete
        case SDLK_DELETE:
        {
            //Get the active line
            Cyb_LineNode *node = (Cyb_LineNode*)Cyb_GetListElm(data->lines,
                data->activeLine);
                
            if(!node)
            {
                return;
            }
            
            //Is the caret at the end?
            if(node->caretPos == node->line->len - 1)
            {
                //Ignore this if the active line is the last line
                if(data->activeLine > data->lines->len - 2)
                {
                    return;
                }
                
                //Add the next line to the end of this line and remove the next
                //line
                Cyb_InsertText(textBox, data->activeLine, CYB_VEC_END,
                    (char*)((Cyb_LineNode*)node->base.next)->line->data);
                Cyb_RemoveLine(textBox, data->activeLine + 1);
            }
            //Is the caret elsewhere?
            else
            {
                Cyb_RemoveText(textBox, data->activeLine, node->caretPos, 1);
            }
            
            break;
        }
        
            //Return
        case SDLK_RETURN:
        {
            //Get the active line
            Cyb_LineNode *node = (Cyb_LineNode*)Cyb_GetListElm(data->lines,
                data->activeLine);
                
            if(!node)
            {
                return;
            }
            
            //Is the caret at the beginning?
            if(node->caretPos == 0)
            {
                Cyb_InsertLine(textBox, data->activeLine++);
            }
            //Is the caret at the end?
            else if(node->caretPos == node->line->len - 1)
            {
                Cyb_InsertLine(textBox, ++data->activeLine);
            }
            //Is the caret in the middle?
            else
            {
                //Get a pointer to the text to move to the next line and insert
                //the new line
                int start = node->caretPos;
                char *tmp = &((char*)node->line->data)[start];
                Cyb_InsertLine(textBox, ++data->activeLine);
                node = (Cyb_LineNode*)Cyb_GetListElm(data->lines, data->activeLine);
                
                if(!node)
                {
                    return;
                }
                
                //Insert the text after the caret into the new line and remove it
                //from the original line
                Cyb_InsertText(textBox, data->activeLine, CYB_VEC_START, tmp);
                Cyb_RemoveText(textBox, data->activeLine - 1, start, strlen(tmp));
            }
            
            break;
        }
        }
        
        break;
    }
    
        //Text Input Event
    case SDL_TEXTINPUT:
    {
        //Ignore this unless the widget has focus and is not read-only
        if(Cyb_GetActiveGrid() != textBox || data->mode & CYB_TEXTBOX_READONLY)
        {
            return;
        }
        
        //Get the active line
        Cyb_LineNode *node = (Cyb_LineNode*)Cyb_GetListElm(data->lines,
            data->activeLine);
                
        if(!node)
        {
            return;
        }
        
        //Insert what was typed
        Cyb_InsertText(textBox, data->activeLine, node->caretPos++, 
            event->text.text);
        break;
    }
    }
}


Cyb_Grid *Cyb_CreateTextBox(void)
{
    //Allocate new text box
    Cyb_Grid *textBox = Cyb_CreateGrid();
    
    if(!textBox)
    {
        return NULL;
    }
    
    //Initialize the text box
    textBox->base.type = CYB_TEXTBOX;
    textBox->draw = &Cyb_DrawTextBoxProc;
    textBox->handleEvent = &Cyb_HandleTextBoxEventProc;
    textBox->data = Cyb_CreateObject(sizeof(Cyb_TextBoxData),
        (Cyb_FreeProc)&Cyb_FreeTextBoxData, CYB_TEXTBOX);
        
    if(!textBox->data)
    {
        Cyb_FreeObject((Cyb_Object**)&textBox);
        return NULL;
    }
    
    Cyb_TextBoxData *data = (Cyb_TextBoxData*)textBox->data;
    data->mode = 0;
    data->caretTmr = 30;
    data->caretVisible = TRUE;
    data->scrollPos.x = 0;
    data->scrollPos.y = 0;
    data->isScrolling = FALSE;
    data->activeLine = 0;
    data->lines = Cyb_CreateList(sizeof(Cyb_LineNode),
        (Cyb_FreeNodeProc)&Cyb_FreeLineNode);
        
    if(!data->lines)
    {
        Cyb_FreeObject((Cyb_Object**)&textBox);
        return NULL;
    }
    
    Cyb_InsertLine(textBox, CYB_LIST_START);
    return textBox;
}


void Cyb_SetTextBoxMode(Cyb_Grid *textBox, int mode)
{
    //Ensure that the widget is a text box
    if(textBox->base.type != CYB_TEXTBOX)
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s",
            "[CybUI] The widget passed to 'Cyb_SetTextBoxMode' was not a text box!");
        return;
    }
    
    //Set the new text box mode
    Cyb_TextBoxData *data = (Cyb_TextBoxData*)textBox->data;
    data->mode = mode;
}


void Cyb_ToggleTextBoxMode(Cyb_Grid *textBox, int mode)
{
    //Ensure that the widget is a text box
    if(textBox->base.type != CYB_TEXTBOX)
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s",
            "[CybUI] The widget passed to 'Cyb_ToggleTextBoxMode' was not a text box!");
        return;
    }
    
    //Toggle the text box mode
    Cyb_TextBoxData *data = (Cyb_TextBoxData*)textBox->data;
    data->mode ^= mode;
}


void Cyb_SetTextBoxScrollPos(Cyb_Grid *textBox, int x, int y)
{
    //Ensure that the widget is a text box
    if(textBox->base.type != CYB_TEXTBOX)
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s",
            "[CybUI] The widget passed to 'Cyb_SetTextBoxScrollPos' was not a text box!");
        return;
    }
    
    //Set the current scroll pos
    Cyb_TextBoxData *data = (Cyb_TextBoxData*)textBox->data;
    data->scrollPos.x = x;
    data->scrollPos.y = y;
}


void Cyb_SetCaretPos(Cyb_Grid *textBox, int line, int col)
{
    //Ensure that the widget is a text box
    if(textBox->base.type != CYB_TEXTBOX)
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s",
            "[CybUI] The widget passed to 'Cyb_SetCaretPos' was not a text box!");
        return;
    }
    
    //Set the current line
    Cyb_TextBoxData *data = (Cyb_TextBoxData*)textBox->data;
    Cyb_LineNode *node = (Cyb_LineNode*)Cyb_GetListElm(data->lines, line);
    
    if(!node)
    {
        return;
    }
    
    data->activeLine = line;
    
    //Set the current column
    if(col <= node->line->len)
    {
        node->caretPos = col;
    }
}


void Cyb_InsertLine(Cyb_Grid *textBox, int line)
{
    //Ensure that the widget is a text box
    if(textBox->base.type != CYB_TEXTBOX)
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s",
            "[CybUI] The widget passed to 'Cyb_InsertLine' was not a text box!");
        return;
    }
    
    //Only supported in multiline mode!
    Cyb_TextBoxData *data = (Cyb_TextBoxData*)textBox->data;
    
    if(!(data->mode & CYB_TEXTBOX_MULTILINE) && data->lines->len != 0)
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "[CybUI] Can only insert lines in multiline mode!");
        return;
    }
    
    //Insert the new line
    Cyb_LineNode *node = (Cyb_LineNode*)Cyb_InsertListElm(data->lines, line);
    
    if(!node)
    {
        return;
    }
    
    node->caretPos = 0;
    node->line = Cyb_CreateVec(sizeof(char), NULL);
    
    if(!node->line)
    {
        Cyb_RemoveListElm(data->lines, line);
        return;
    }
    
    //NULL-terminate the new line
    char *c = Cyb_InsertVecElm(node->line, CYB_VEC_END);
    
    if(!c)
    {
        Cyb_RemoveLine(textBox, line);
        return;
    }
    
    *c = 0;
}


void Cyb_RemoveLine(Cyb_Grid *textBox, int line)
{
    //Ensure that the widget is a text box
    if(textBox->base.type != CYB_TEXTBOX)
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s",
            "[CybUI] The widget passed to 'Cyb_RemoveLine' was not a text box!");
        return;
    }
    
    //Only supported in multiline mode!
    Cyb_TextBoxData *data = (Cyb_TextBoxData*)textBox->data;
    
    if(!(data->mode & CYB_TEXTBOX_MULTILINE))
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s", 
            "[CybUI] Can only insert lines in multiline mode!");
        return;
    }
    
    //Remove the given line
    if(data->lines->len > 1)
    {
        Cyb_RemoveListElm(data->lines, line);
        
        if(data->activeLine > data->lines->len - 1)
        {
            data->activeLine = data->lines->len - 1;
        }
    }
}


const char *Cyb_GetLine(Cyb_Grid *textBox, int line)
{
    //Ensure that the widget is a text box
    if(textBox->base.type != CYB_TEXTBOX)
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s",
            "[CybUI] The widget passed to 'Cyb_GetLine' was not a text box!");
        return NULL;
    }
    
    //Get the requested line
    Cyb_TextBoxData *data = (Cyb_TextBoxData*)textBox->data;
    Cyb_LineNode *node = (Cyb_LineNode*)Cyb_GetListElm(data->lines, line);
    
    if(!node)
    {
        return NULL;
    }
    
    return node->line->data;
}


void Cyb_InsertText(Cyb_Grid *textBox, int line, int col, const char *text)
{
    //Ensure that the widget is a text box
    if(textBox->base.type != CYB_TEXTBOX)
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s",
            "[CybUI] The widget passed to 'Cyb_InsertText' was not a text box!");
        return;
    }
    
    //Get the line to insert text into
    Cyb_TextBoxData *data = (Cyb_TextBoxData*)textBox->data;
    Cyb_LineNode *node = (Cyb_LineNode*)Cyb_GetListElm(data->lines, line);
    
    if(!node)
    {
        return;
    }
    
    //Insert the text
    int curLine = (line < 0 ? line + data->lines->len : line);
    int curCol = (col < 0 ? col + node->line->len : col);
    
    for(const char *pos = text; *pos; pos++)
    {
        //Handle newline chars
        if(*pos == '\n')
        {
            Cyb_InsertLine(textBox, ++curLine);
            node = (Cyb_LineNode*)Cyb_GetListElm(data->lines, curLine);
            
            if(!node)
            {
                return;
            }
        
            curCol = 0;
            continue;
        }
        else if(*pos == '\r')
        {
            continue;
        }
        
        //Insert next char
        char *c = (char*)Cyb_InsertVecElm(node->line, curCol++);
        
        if(!c)
        {
            return;
        }
        
        *c = *pos;
    }
}


void Cyb_RemoveText(Cyb_Grid *textBox, int line, int col, int count)
{
    //Ensure that the widget is a text box
    if(textBox->base.type != CYB_TEXTBOX)
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s",
            "[CybUI] The widget passed to 'Cyb_RemoveText' was not a text box!");
        return;
    }
    
    //Get the line to remove text from
    Cyb_TextBoxData *data = (Cyb_TextBoxData*)textBox->data;
    Cyb_LineNode *node = (Cyb_LineNode*)Cyb_GetListElm(data->lines, line);
    
    if(!node)
    {
        return;
    }
    
    //Remove the text
    for(int i = 0; i < count; i++)
    {
        //Remove next char
        Cyb_RemoveVecElm(node->line, col);
    }
    
    if(node->caretPos > node->line->len - 1)
    {
        node->caretPos = node->line->len - 1;
    }
}


void Cyb_ClearText(Cyb_Grid *textBox)
{
    //Ensure that the widget is a text box
    if(textBox->base.type != CYB_TEXTBOX)
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s",
            "[CybUI] The widget passed to 'Cyb_ClearText' was not a text box!");
        return;
    }
    
    //Remove all lines
    Cyb_TextBoxData *data = (Cyb_TextBoxData*)textBox->data;
    
    while(data->lines->len)
    {
        Cyb_RemoveListElm(data->lines, CYB_LIST_END);
    }
    
    //Add a single new line
    Cyb_InsertLine(textBox, CYB_LIST_START);
}


void Cyb_LoadTextRW(Cyb_Grid *textBox, SDL_RWops *file, int doClose)
{
    //Ensure that the widget is a text box
    if(textBox->base.type != CYB_TEXTBOX)
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s",
            "[CybUI] The widget passed to 'Cyb_LoadText' was not a text box!");
            
        //Close the file?
        if(doClose)
        {
            SDL_RWclose(file);
        }
        
        return;
    }
    
    //Ignore this if the file is NULL
    if(!file)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[CybUI] %s", SDL_GetError());
        return;
    }
    
    //Clear the text boxoad the contents of the file
    Cyb_ClearText(textBox);
    
    //Load the file
    while(SDL_RWread(file, lineBuf, sizeof(char), sizeof(lineBuf)) == 
        sizeof(lineBuf))
    {
        Cyb_InsertText(textBox, CYB_LIST_END, CYB_VEC_END, lineBuf);
    }
    
    Cyb_InsertText(textBox, CYB_LIST_END, CYB_VEC_END, lineBuf);
    
    //Close the file?
    if(doClose)
    {
        SDL_RWclose(file);
    }
}


void Cyb_SaveTextRW(Cyb_Grid *textBox, SDL_RWops *file, int doClose)
{
    //Ensure that the widget is a text box
    if(textBox->base.type != CYB_TEXTBOX)
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s",
            "[CybUI] The widget passed to 'Cyb_SaveText' was not a text box!");
            
        //Close the file?
        if(doClose)
        {
            SDL_RWclose(file);
        }
        
        return;
    }
    
    //Ignore this if the file is NULL
    if(!file)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[CybUI] %s", SDL_GetError());
        return;
    }
    
    //Save to the file
    Cyb_TextBoxData *data = (Cyb_TextBoxData*)textBox->data;
    char lf = '\n';
    
    for(Cyb_LineNode *node = (Cyb_LineNode*)data->lines->first; node;
        node = (Cyb_LineNode*)node->base.next)
    {
        SDL_RWwrite(file, (const char*)node->line->data, sizeof(char), 
            node->line->len - 1);
        SDL_RWwrite(file, &lf, sizeof(char), 1);
    }
    
    //Close the file?
    if(doClose)
    {
        SDL_RWclose(file);
    }
}