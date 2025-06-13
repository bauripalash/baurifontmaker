#include "include/uiopts.h"
#include "include/balloc.h"
#include "include/defaults.h"
#include "include/ext/raylib.h"
#include "include/itemselector.h"
#include "include/toolbar.h"
#include "include/windows/edititem.h"
#include "include/windows/newitem.h"
#include <stdbool.h>

UiConfig *NewUiConfig() {
    UiConfig *uc = (UiConfig *)bcalloc(1, sizeof(UiConfig));
    if (uc == NULL) {
        TraceLog(LOG_ERROR, "Failed to allocate memory for new Ui Config");
        return NULL;
    }
    uc->enableGrid = true;
    uc->gridBtnSize = DEF_GRID_BTN_SIZE;
    uc->gridThickness = DEF_GRID_THICKNESS;
    uc->toolbarHeight = DEF_TOOLBAR_HEIGHT;
    uc->statusbarHeight = DEF_STATUSBAR_HEIGHT;
    uc->itemListWidth = DEF_ITEMLIST_WIDTH;
    uc->gridSize = (Vector2){.x = DEF_CANVAS_COLUMN, .y = DEF_CANVAS_ROW};
    uc->isPopupActive = false;

    return uc;
}
void FreeUiConfig(UiConfig *conf) {
    if (conf != NULL) {
        bfree(conf);
    }
}

UiStates *NewUiStates() {
    UiStates *states = (UiStates *)bcalloc(1, sizeof(UiStates));
    if (states == NULL) {
        TraceLog(LOG_ERROR, "Failed to allocate for new UI states");
        return NULL;
    }
    states->newItem = CreateNewItemState();
    states->editItem = CreateEditItemState();
    states->toolbar = CreateToolbar(DEF_TOOLBAR_HEIGHT);
    states->itemSelector = CreateItemSelector(
        DEF_ITEMLIST_WIDTH, DEF_TOOLBAR_HEIGHT, DEF_STATUSBAR_HEIGHT
    );

    return states;
}
void FreeUiStates(UiStates *states) {
    if (states != NULL) {
        bfree(states);
    }
}
