#include "../include/objects/uistates.h"
#include "../include/widgets/canvas.h"
#include "../include/balloc.h"
#include "../include/defaults.h"

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

    states->settings = CreateSettingsState();
    states->canvas = CreateCanvas();


    return states;
}
void FreeUiStates(UiStates *states) {
    if (states != NULL) {
        bfree(states);
    }
}
