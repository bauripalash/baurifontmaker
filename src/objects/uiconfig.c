#include "../include/objects/uiconfig.h"
#include "../include/balloc.h"
#include "../include/colors.h"
#include "../include/defaults.h"

UiConfig *NewUiConfig() {
    UiConfig *uc = (UiConfig *)bcalloc(1, sizeof(UiConfig));
    if (uc == NULL) {
        TraceLog(LOG_ERROR, "Failed to allocate memory for new Ui Config");
        return NULL;
    }
    uc->enableGrid = true;
    uc->gridBtnSize = DEF_GRID_BTN_SIZE;
    uc->toolbarHeight = DEF_TOOLBAR_HEIGHT;
    uc->statusbarHeight = DEF_STATUSBAR_HEIGHT;
    uc->itemListWidth = DEF_ITEMLIST_WIDTH;
    uc->gridSize = (Vector2){.x = DEF_CANVAS_COLUMN, .y = DEF_CANVAS_ROW};
    uc->gridColor = ColorPrimary;
    uc->canvasCellColor = ColorBackground;
    uc->canvasFillColor = ColorPrimary;
    uc->isPopupActive = false;

    return uc;
}
void FreeUiConfig(UiConfig *conf) {
    if (conf != NULL) {
        bfree(conf);
    }
}
