#ifndef CYBLISTBOX_H
#define CYBLISTBOX_H

/** @file
 * @brief CybUI - List Box API
 */
 
#include "CybGrid.h"
#include "CybObjects.h"

 
#ifdef __cplusplus
extern "C" {
#endif

//Enums
//=================================================================================
enum Cyb_ListBoxMode
{
    CYB_LISTBOX_MULTISELECT = 0x1
};


//Structures
//=================================================================================
typedef struct
{
    Cyb_ListNode base;
    int isSelected;
    char *name;
    Cyb_Object *data;
} Cyb_ListBoxItem;


//Functions
//=================================================================================
CYBAPI void Cyb_DrawListBoxProc(Cyb_Grid *listBox, SDL_Renderer *renderer);
CYBAPI void Cyb_HandleListBoxEventProc(Cyb_Grid *listBox, const SDL_Event *event);
CYBAPI Cyb_Grid *Cyb_CreateListBox(void);
CYBAPI void Cyb_SetListBoxMode(Cyb_Grid *listBox, int mode);
CYBAPI void Cyb_ToggleListBoxMode(Cyb_Grid *listBox, int mode);
CYBAPI void Cyb_SetListBoxScrollPos(Cyb_Grid *listBox, int x, int y);
CYBAPI void Cyb_InsertItem(Cyb_Grid *listBox, int i, const char *name, 
    Cyb_Object *itemData);
CYBAPI void Cyb_RemoveItem(Cyb_Grid *listBox, int i);
CYBAPI Cyb_ListBoxItem *Cyb_GetItem(Cyb_Grid *listBox, int i);

#ifdef __cplusplus
}
#endif

#endif