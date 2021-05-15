/*
CybUI - Widget List API
*/

#include "CybGrid.h"
#include "CybObjects.h"
#include "CybWidgetList.h"


//Functions
//=================================================================================
void Cyb_FreeWidgetNode(Cyb_WidgetNode *node)
{
    //Free the widget
    Cyb_FreeObject((Cyb_Object**)&node->widget);
}


Cyb_List *Cyb_CreateWidgetList(void)
{
    return Cyb_CreateList(sizeof(Cyb_WidgetNode), 
        (Cyb_FreeNodeProc)&Cyb_FreeWidgetNode);
}