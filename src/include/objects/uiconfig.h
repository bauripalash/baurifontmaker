#ifndef UICONFIG_H
#define UICONFIG_H

#include "../ext/raylib.h"

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

UiConfig *NewUiConfig();
void FreeUiConfig(UiConfig *conf);

#endif
