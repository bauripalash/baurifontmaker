#include "../include/objects/uistates.h"
#include "../include/balloc.h"
#include "../include/defaults.h"
#include "../include/widgets/canvas.h"

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
    states->infoEdit = CreateInfoEditState();

    return states;
}

bool WindowPopupActive(UiStates *states) {
    return (
        states->newItem.windowActive || states->editItem.windowActive ||
        states->settings.windowActive || states->infoEdit.windowActive
    );
}

void FreeUiStates(UiStates *states) {
    if (states != NULL) {
        bfree(states);
    }
}
