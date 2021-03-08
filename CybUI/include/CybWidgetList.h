#ifndef CYBWIDGETLIST_H
#define CYBWIDGETLIST_H

/** @file
 * @brief CybUI - Widget List API
 */
 
#include "CybCommon.h"

 
#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup CybUI
 * @brief Cybermals Engine - UI Subsystem
 * @{
 */
 
//Structures
//=================================================================================
/** @brief A list node containing a widget pointer.
 */
typedef struct
{
    Cyb_ListNode base; /**< Base object. (read-only) */
    Cyb_Grid *widget;  /**< Widget pointer. (read-only) */
} Cyb_WidgetNode;


//Functions
//=================================================================================
/** @brief Create a widget list.
 *
 * @return Pointer to the widget list.
 */
CYBAPI Cyb_List *Cyb_CreateWidgetList(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif