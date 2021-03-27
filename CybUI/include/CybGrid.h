#ifndef CYBGRID_H
#define CYBGRID_H

/** @file
 * @brief CybUI - Grid API
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "CybCommon.h"
#include "CybObjects.h"

 
#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup CybUI
 * @brief Cybermals Engine - UI Subsystem
 * @{
 */

//Types
//=================================================================================
/** @brief Grid widget type.
 */
typedef struct Cyb_Grid Cyb_Grid;

/** @brief Widget draw procedure.
 *
 * @param grid Pointer to the widget.
 * @param renderer Pointer to the renderer.
 */
typedef void (*CybWidgetDrawProc)(Cyb_Grid *grid, SDL_Renderer *renderer);

/** @brief Widget event handler procedure.
 *
 * @param grid Pointer to the widget.
 * @param event Pointer to the event.
 */
typedef void (*CybWidgetEventProc)(Cyb_Grid *grid, const SDL_Event *event);


//Structures
//=================================================================================
/** @brief Grid widget structure.
 */
struct Cyb_Grid
{
    Cyb_Object base;                /**< Base object. (read-only) */
    CybWidgetDrawProc draw;         /**< Draw procedure. (read-only) */
    CybWidgetEventProc handleEvent; /**< Event handler procedure. (read-only) */
    Cyb_Grid *parent;               /**< Parent widget pointer. (read-only) */
    Cyb_List *children;             /**< Child widget list. (read-only) */
    int rows;                       /**< Row count. */
    int cols;                       /**< Column count. */
    int row;                        /**< Row within the parent. */
    int col;                        /**< Column within the parent. */
    int rowspan;                    /**< Number of rows to span. */
    int colspan;                    /**< Number or columns to span. */
    int visible;                    /**< Visibility state. */
    SDL_Rect viewport;              /**< Cached local viewport. (read-only) */
    char *id;                       /**< Widget ID. (read-only) */
    SDL_Texture *bg;                /**< Widget background texture. */
    SDL_Color fg;                   /**< Widget foreground color. */
    TTF_Font *font;                 /**< Widget font. */
    char *text;                     /**< Widget-specific text data. (read-only) */
    Cyb_Object *data;               /**< Widget-specific data. (read-only) */
};


//Functions
//=================================================================================
/** @brief Default grid drawing procedure.
 *
 * @param grid Pointer to the grid.
 * @param renderer Pointer to the renderer.
 */
CYBAPI void Cyb_DrawGridProc(Cyb_Grid *grid, SDL_Renderer *renderer);

/** @brief Default grid event handler procedure.
 *
 * @param grid Pointer to the grid.
 * @param event Pointer to the event to handle.
 */
CYBAPI void Cyb_HandleGridEventProc(Cyb_Grid *grid, const SDL_Event *event);

/** @brief Create a new grid widget.
 *
 * @return Pointer to the grid.
 */
CYBAPI Cyb_Grid *Cyb_CreateGrid(void);

/** @brief Add a child to a given grid widget.
 *
 * @param parent Pointer to the parent grid.
 * @param child Pointer to the child grid.
 */
CYBAPI void Cyb_AddChildGrid(Cyb_Grid *parent, Cyb_Grid *child);

/** @brief Set the ID of the given grid widget.
 *
 * @param grid Pointer to the grid.
 * @param id The new ID string.
 */
CYBAPI void Cyb_SetGridID(Cyb_Grid *grid, const char *id);

/** @brief Set the text of the given grid widget.
 *
 * @param grid Pointer to the grid.
 * @param text The new text.
 */
CYBAPI void Cyb_SetGridText(Cyb_Grid *grid, const char *text);

/** @brief Get a grid widget by its ID.
 *
 * @param grid Pointer to the grid.
 * @param id The ID string to search for.
 *
 * @return Pointer to the requested grid or NULL on failure.
 */
CYBAPI Cyb_Grid *Cyb_GetGridByID(Cyb_Grid *grid, const char *id);

/** @brief Draw a given widget tree.
 *
 * @param grid Pointer to the grid widget at the base of the widget tree.
 * @param renderer Pointer to the renderer to use for drawing.
 */
CYBAPI void Cyb_DrawUI(Cyb_Grid *grid, SDL_Renderer *renderer);

/** @brief Handle a UI event.
 *
 * @param grid Pointer to the grid widget at the base of the widget tree.
 * @param event Pointer to the event to handle.
 */
CYBAPI void Cyb_HandleUIEvent(Cyb_Grid *grid, const SDL_Event *event);

/** @brief Get the active grid.
 *
 * @return Pointer to the active grid.
 */
CYBAPI Cyb_Grid *Cyb_GetActiveGrid(void);

/** @brief Convert from global to local coordinates.
 *
 * @param grid Pointer to the grid.
 * @param in Pointer to the point to convert.
 * @param out Pointer to the point to write the result into.
 */
CYBAPI void Cyb_GlobalToLocal(Cyb_Grid *grid, const SDL_Point *in,
    SDL_Point *out);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif