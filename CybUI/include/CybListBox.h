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

/** @addtogroup CybUI
 * @brief Cybermals Engine - UI Subsystem
 * @{
 */

//Enums
//=================================================================================
/** @brief List box modes.
 */
enum Cyb_ListBoxMode
{
    CYB_LISTBOX_MULTISELECT = 0x1 /**< Multi-select mode. */
};


//Structures
//=================================================================================
/** @brief List box item.
 */
typedef struct
{
    Cyb_ListNode base; /**< Base object. (read-only) */
    int isSelected;    /**< Selection state flag. */
    char *name;        /**< Item name. (read-only) */
    Cyb_Object *data;  /**< Item data. */
} Cyb_ListBoxItem;


//Functions
//=================================================================================
/** @brief List box drawing procedure.
 *
 * @param listBox Pointer to the list box.
 * @param renderer Pointer to the renderer to use.
 */
CYBAPI void Cyb_DrawListBoxProc(Cyb_Grid *listBox, SDL_Renderer *renderer);

/** @brief List box event handler procedure.
 *
 * @param listBox Pointer to the list box.
 * @param event Pointer to the event to handle.
 */
CYBAPI void Cyb_HandleListBoxEventProc(Cyb_Grid *listBox, const SDL_Event *event);

/** @brief Create a new list box.
 *
 * @return Pointer to the list box.
 */
CYBAPI Cyb_Grid *Cyb_CreateListBox(void);

/** @brief Set the mode of a list box.
 *
 * @param listBox Pointer to the list box.
 * @param mode The new mode.
 */
CYBAPI void Cyb_SetListBoxMode(Cyb_Grid *listBox, int mode);

/** @brief Toggle one or more list box modes.
 *
 * @param listBox Pointer to the list box.
 * @param mode The modes to toggle.
 */
CYBAPI void Cyb_ToggleListBoxMode(Cyb_Grid *listBox, int mode);

/** @brief Set the scroll position of a list box.
 *
 * @param listBox Pointer to the list box.
 * @param x The new X coordinate.
 * @param y The new Y coordinate.
 */
CYBAPI void Cyb_SetListBoxScrollPos(Cyb_Grid *listBox, int x, int y);

/** @brief Insert a new item into a list box.
 *
 * @param listBox Pointer to the list box.
 * @param i The index of the item to insert before.
 * @param name The name of the new item.
 * @param itemData Pointer to a user-defined object associated with the new item.
 */
CYBAPI void Cyb_InsertItem(Cyb_Grid *listBox, int i, const char *name, 
    Cyb_Object *itemData);
    
/** @brief Remove an item from a list box.
 *
 * @param listBox Pointer to the list box.
 * @param i The index of the item to remove.
 */
CYBAPI void Cyb_RemoveItem(Cyb_Grid *listBox, int i);

/** @brief Get an item from a list box.
 *
 * @param listBox Pointer to the list box.
 * @param i The index of the item to fetch.
 *
 * @return Pointer to the requested list box item.
 */
CYBAPI Cyb_ListBoxItem *Cyb_GetItem(Cyb_Grid *listBox, int i);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif