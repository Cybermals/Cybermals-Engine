#ifndef CYBTEXTBOX_H
#define CYBTEXTBOX_H

/** @file
 * @brief CybUI - TextBox API
 */
 
#include "CybGrid.h"
 
 
#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup Cyb_UI
 * @brief Cybermals Engine - UI Subsystem
 * @{
 */

//Macros
//=================================================================================
/** @brief Load a text file into a text box.
 *
 * @param textBox Pointer to the text box.
 * @param filename The name of the file to load.
 */
#define Cyb_LoadText(textBox, filename) Cyb_LoadTextRW(textBox, \
    SDL_RWFromFile(filename, "r"), TRUE)
    
/** @brief Save the content of a text box to a file.
 *
 * @param textBox Pointer to the text box.
 * @param filename The name of the file to save.
 */
#define Cyb_SaveText(textBox, filename) Cyb_SaveTextRW(textBox, \
    SDL_RWFromFile(filename, "w"), TRUE)


//Enums
//=================================================================================
/** @brief Text box modes.
 */
enum Cyb_TextBoxMode
{
    CYB_TEXTBOX_MULTILINE = 0x1, /**< Multi-line mode. */
    CYB_TEXTBOX_READONLY = 0x2   /**< Read-only mode. */
};


//Functions
//=================================================================================
/** @brief Text box drawing procedure.
 *
 * @param textBox Pointer to the text box.
 * @param renderer Pointer to the renderer to use.
 */
CYBAPI void Cyb_DrawTextBoxProc(Cyb_Grid *textBox, SDL_Renderer *renderer);

/** @brief Text box event handler procedure.
 *
 * @param textBox Pointer to the text box.
 * @param event Pointer to the event to handle.
 */
CYBAPI void Cyb_HandleTextBoxEventProc(Cyb_Grid *textBox, const SDL_Event *event);

/** @brief Create a new text box.
 *
 * @return Pointer to the text box.
 */
CYBAPI Cyb_Grid *Cyb_CreateTextBox(void);

/** @brief Set the mode of a text box.
 *
 * @param textBox Pointer to the text box.
 * @param mode The new mode.
 */
CYBAPI void Cyb_SetTextBoxMode(Cyb_Grid *textBox, int mode);

/** @brief Toggle one or more text box modes.
 *
 * @param textBox Pointer to the text box.
 * @param mode The mode flags to toggle.
 */
CYBAPI void Cyb_ToggleTextBoxMode(Cyb_Grid *textBox, int mode);

/** @brief Set the scroll position of a text box.
 *
 * @param textBox Pointer to the text box.
 * @param x The new X coordinate.
 * @param y The new Y coordinate.
 */
CYBAPI void Cyb_SetTextBoxScrollPos(Cyb_Grid *textBox, int x, int y);

/** @brief Set the caret position of a text box.
 *
 * @param textBox Pointer to the text box.
 * @param line The new active line.
 * @param col The new active column.
 */
CYBAPI void Cyb_SetCaretPos(Cyb_Grid *textBox, int line, int col);

/** @brief Insert a new line into a text box.
 *
 * @param textBox Pointer to the text box.
 * @param line The index of the line to insert before.
 */
CYBAPI void Cyb_InsertLine(Cyb_Grid *textBox, int line);

/** @brief Remove a line from a text box.
 *
 * @param textBox Pointer to the text box.
 * @param line The index of the line to remove.
 */
CYBAPI void Cyb_RemoveLine(Cyb_Grid *textBox, int line);

/** @brief Get a line from a text box.
 *
 * @param textBox Pointer to the text box.
 * @param line The index of the line to fetch.
 *
 * @return Pointer to the text in the requested line.
 */
CYBAPI const char *Cyb_GetLine(Cyb_Grid *textBox, int line);

/** @brief Insert text into a text box.
 *
 * @param textBox Pointer to the text box.
 * @param line The index of the line to insert text into.
 * @param col The index of the character to insert the text before.
 * @param text The text to insert.
 */
CYBAPI void Cyb_InsertText(Cyb_Grid *textBox, int line, int col, const char *text);

/** @brief Remove text from a text box.
 *
 * @param textBox Pointer to the text box.
 * @param line The index of the line to remove text from.
 * @param col The index of the first character to remove.
 * @param count The number of characters to remove.
 */
CYBAPI void Cyb_RemoveText(Cyb_Grid *textBox, int line, int col, int count);

/** @brief Remove the entire contents of a text box.
 *
 * @param textBox Pointer to the text box.
 */
CYBAPI void Cyb_ClearText(Cyb_Grid *textBox);

/** @brief Load text from an open file into a text box.
 *
 * @param textBox Pointer to the text box.
 * @param file Pointer to the file.
 * @param doClose TRUE if the file should be closed afterward.
 */
CYBAPI void Cyb_LoadTextRW(Cyb_Grid *textBox, SDL_RWops *file, int doClose);

/** @brief Save text from a text box into a given file.
 *
 * @param textBox Pointer to the text box.
 * @param file Pointer to the file.
 * @param doClose TRUE if the file should be closed afterward.
 */
CYBAPI void Cyb_SaveTextRW(Cyb_Grid *textBox, SDL_RWops *file, int doClose);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif