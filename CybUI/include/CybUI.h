#ifndef CYBUI_H
#define CYBUI_H

/** @file
 * @brief CybUI - Main API
 */
 
#include "CybButton.h"
#include "CybCommon.h"
#include "CybGrid.h"
#include "CybTextBox.h"
#include "CybTimer.h"
#include "CybUIEvents.h"
#include "CybUILoader.h"
#include "CybWidgetList.h"

 
#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup CybUI
 * @brief Cybermals Engine - UI Subsystem
 * @{
 */

//Functions
//=================================================================================
/** @brief Initialize the UI subsystem.
 *
 * @return CYB_NO_ERROR on success or CYB_ERROR on failure.
 */
CYBAPI int Cyb_InitUI(void);

/** @brief Finalize the UI subsystem.
 */
CYBAPI void Cyb_FiniUI(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif