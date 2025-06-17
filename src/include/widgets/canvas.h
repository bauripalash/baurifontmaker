#ifndef CANVAS_H
#define CANVAS_H

#include "../objects/fontitem.h"
#include "../objects/uiconfig.h"

#define CANVAS_PANEL_MARGIN 20

#include <stdbool.h>
typedef struct {
    int hoverPosX;
    int hoverPosY;
    float zoomValue;
    float minZoom;
    float maxZoom;

} CanvasState;

CanvasState CreateCanvas();
void Canvas(CanvasState *state, const UiConfig *config, FontItem *item);

#endif
