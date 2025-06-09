#ifndef _GUI_H_
#define _GUI_H_

#include <stdbool.h>
#include <stdint.h>

#include "ext/raylib.h"
#include "fontitem.h"
#include "fontitemlist.h"
#include "uiopts.h"

typedef struct Gui {
    int winWidth;
    int winHeight;
    const char *title;
    UiStates *states;
    UiConfig *conf;

    char openFilename[512];
    char saveFilename[512];

    Vector2 itemListAnchor;
    FontItemList *items;
    FontItem *currentItem;

} Gui;

Gui *NewGUI();
void FreeGui(Gui *ui);
void GuiMain();

#endif
