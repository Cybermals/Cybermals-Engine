#ifndef CYBTEXTBOX_H
#define CYBTEXTBOX_H

/** @file
 * @brief CybUI - TextBox API
 */
 
#include "CybGrid.h"
 
 
#ifdef __cplusplus
extern "C" {
#endif

//Enums
//=================================================================================
enum Cyb_TextBoxMode
{
    CYB_TEXTBOX_MULTILINE = 0x1,
    CYB_TEXTBOX_READONLY = 0x2
};


//Functions
//=================================================================================
CYBAPI void Cyb_DrawTextBoxProc(Cyb_Grid *textBox, SDL_Renderer *renderer);
CYBAPI void Cyb_HandleTextBoxEventProc(Cyb_Grid *textBox, const SDL_Event *event);
CYBAPI Cyb_Grid *Cyb_CreateTextBox(void);
CYBAPI void Cyb_SetTextBoxMode(Cyb_Grid *textBox, int mode);
CYBAPI void Cyb_SetScrollPos(Cyb_Grid *textBox, int x, int y);
CYBAPI void Cyb_SetCaretPos(Cyb_Grid *textBox, int line, int col);
CYBAPI void Cyb_InsertLine(Cyb_Grid *textBox, int line);
CYBAPI void Cyb_RemoveLine(Cyb_Grid *textBox, int line);
CYBAPI void Cyb_InsertText(Cyb_Grid *textBox, int line, int col, const char *text);
CYBAPI void Cyb_RemoveText(Cyb_Grid *textBox, int line, int col, int count);

#ifdef __cplusplus
}
#endif

#endif