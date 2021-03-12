/*
CybUI - TextBox API
*/

#include "CybObject.h"
#include "CybList.h"
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
    SDL_Point scrollPos;
    int activeLine;
    Cyb_List *lines;
} Cyb_TextBoxData;


//Functions
//=================================================================================
void Cyb_FreeLineNode(Cyb_LineNode *node)
{
    //Free the line vector
    Cyb_FreeObject((Cyb_Object**)&node->line);
}


void Cyb_FreeListBoxData(Cyb_TextBoxData *obj)
{
    //Free the lines list
    Cyb_FreeObject((Cyb_Object**)&obj->lines);
}


void Cyb_DrawTextBoxProc(Cyb_Grid *textBox, SDL_Renderer *renderer)
{
    //Call the base draw procedure
    Cyb_DrawGridProc(textBox, renderer);
}


void Cyb_HandleTextBoxEventProc(Cyb_Grid *textBox, const SDL_Event *event)
{
    //Call the base event handler procedure
    Cyb_HandleGridEventProc(textBox, event);
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
        (CybFreeProc)&Cyb_FreeListBoxData, CYB_TEXTBOX);
        
    if(!textBox->data)
    {
        Cyb_FreeObject((Cyb_Object**)&textBox);
        return NULL;
    }
    
    Cyb_TextBoxData *data = (Cyb_TextBoxData*)textBox->data;
    data->mode = 0;
    data->scrollPos.x = 0;
    data->scrollPos.y = 0;
    data->activeLine = 0;
    data->lines = Cyb_CreateList(sizeof(Cyb_LineNode),
        (CybFreeNodeProc)&Cyb_FreeLineNode);
        
    if(!data->lines)
    {
        Cyb_FreeObject((Cyb_Object**)&textBox);
        return NULL;
    }
    
    Cyb_InsertLine(textBox, 0);
    return textBox;
}


void Cyb_SetTextBoxMode(Cyb_Grid *textBox, int mode)
{
    //Set the new text box mode
    Cyb_TextBoxData *data = (Cyb_TextBoxData*)textBox->data;
    data->mode = mode;
}


void Cyb_SetScrollPos(Cyb_Grid *textBox, int x, int y)
{
    //Set the current scroll pos
    Cyb_TextBoxData *data = (Cyb_TextBoxData*)textBox->data;
    data->scrollPos.x = x;
    data->scrollPos.y = y;
}


void Cyb_SetCaretPos(Cyb_Grid *textBox, int line, int col)
{
    //Set the current line
    Cyb_TextBoxData *data = (Cyb_TextBoxData*)textBox->data;
    Cyb_LineNode *node = (Cyb_LineNode*)Cyb_GetListElm(data->lines, line);
    
    if(!node)
    {
        return;
    }
    
    data->activeLine = line;
    
    //Set the current column
    if(col < node->line->len)
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
    
    if(!(data->mode & CYB_TEXTBOX_MULTILINE))
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
    for(const char *pos = text; *pos; pos++)
    {
        //Insert next char
        char *c = (char*)Cyb_InsertVecElm(node->line, col);
        
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