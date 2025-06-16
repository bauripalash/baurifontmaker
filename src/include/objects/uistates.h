#ifndef UISTATES_H
#define UISTATES_H

#include "../widgets/canvas.h"
#include "../widgets/itemselector.h"
#include "../widgets/toolbar.h"
#include "../windows/edititem.h"
#include "../windows/newitem.h"
#include "../windows/settings.h"

typedef struct {
    ToolbarState toolbar;
    ItemSelectorState itemSelector;
    NewItemState newItem;
    EditItemState editItem;
    SettingsState settings;
    CanvasState canvas;
} UiStates;

UiStates *NewUiStates();
void FreeUiStates(UiStates *states);

#endif
