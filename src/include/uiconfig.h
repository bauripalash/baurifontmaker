#ifndef UICONFIG_H
#define UICONFIG_H

#include "../external/raylib.h"

typedef struct {
    int gridBtnSize;
    bool enableGrid;
    Vector2 gridSize;
    Color gridColor;

    float toolbarHeight;
    float statusbarHeight;

    float itemListWidth;
    bool isPopupActive;

    Color canvasCellColor;
    Color canvasFillColor;

} UiConfig;

UiConfig *NewUiConfig();
void FreeUiConfig(UiConfig *conf);

#endif
