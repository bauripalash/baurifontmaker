#ifndef UI_OPTS_H
#define UI_OPTS_H

#include <stdbool.h>

#include "ext/raylib.h"
#include "itemselector.h"
#include "toolbar.h"
#include "windows/edititem.h"
#include "windows/newitem.h"

typedef struct {
    int gridBtnSize;
    bool enableGrid;
    int gridThickness;
    Vector2 gridSize;
    Color gridColor;

    float toolbarHeight;
    float statusbarHeight;

    float itemListWidth;
    bool isPopupActive;

    Color canvasCellColor;
    Color canvasFillColor;

} UiConfig;

typedef struct {
    ToolbarState toolbar;
    ItemSelectorState itemSelector;
    NewItemState newItem;
    EditItemState editItem;
} UiStates;

UiConfig *NewUiConfig();
void FreeUiConfig(UiConfig *conf);

UiStates *NewUiStates();
void FreeUiStates(UiStates *states);

#endif
