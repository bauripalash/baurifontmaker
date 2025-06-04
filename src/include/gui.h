#ifndef _GUI_H_
#define _GUI_H_

#include "ext/mlib/m-dict.h"
#include "ext/raylib.h"
#include "fontitem.h"
#include "strlist.h"

#include <stdbool.h>
#include <stdint.h>

DICT_SET_DEF(flipset, uint8_t)

typedef struct {
    int gridBtnSize;
    bool enableGrid;
    int gridThickness;
    Vector2 gridSize;

    float toolbarHeight;
    float statusbarHeight;

    float itemListWidth;

} UiConfig;

typedef struct Gui {
    int winWidth;
    int winHeight;
    const char *title;
    UiConfig *conf;
    flipset_t flipped;
    Vector2 itemListAnchor;
    int itemListIndex;
    int itemListActive;
    StrList *listItems;
    FontItemList *items;
    FontItem *currentItem;

} Gui;

Gui *NewGUI();
void FreeGui(Gui *ui);
void GuiMain();

#endif
