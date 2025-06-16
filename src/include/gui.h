#ifndef _GUI_H_
#define _GUI_H_

#include <stdbool.h>
#include <stdint.h>

#include "ext/raylib.h"
#include "objects/fontitem.h"
#include "objects/fontitemlist.h"
#include "objects/uiconfig.h"
#include "objects/uistates.h"

typedef enum {
    PRELIM_OK = 0,
    PRELIM_GUI_ALLOC_FAILED,
    PRELIM_CONFIG_ALLOC_FAILED,
    PRELIM_FONTLIST_ALLOC_FAILED,
    PRELIM_FONTITEM_ALLOC_FAILED,
    PRELIM_STATES_ALLOC_FAILED,
} GuiPrelimError;

typedef enum {
    APPERR_OK = 0,
    APPERR_NEWITEM_ALLOC,
    APPERR_FONTLIST_ADD,
} AppError;

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
    AppError apperr;

} Gui;

Gui *NewGUI(GuiPrelimError *err);
void FreeGui(Gui *ui);
void GuiMain();

#endif
