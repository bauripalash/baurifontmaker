#include "../include/widgets/canvas.h"
#include "../include/ext/raygui.h"
#include "../include/objects/fontitem.h"
#include "../include/widgets/itemselector.h"
#include <stdbool.h>

CanvasState CreateCanvas() {
    CanvasState canvas = {0};
    canvas.hoverPosX = 0;
    canvas.hoverPosY = 0;
    return canvas;
}

static bool isButtonHovered(Rectangle rect) {
    if (GuiIsLocked()) {
        return false;
    }

    return CheckCollisionPointRec(GetMousePosition(), rect);
}

static bool isButtonClicked(Rectangle rect) {
    if (GuiIsLocked()) {
        return false;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
        CheckCollisionPointRec(GetMousePosition(), rect)) {
        return true;
    }

    return false;
}

void canvasDrawArea(
    CanvasState *state, const UiConfig *config, FontItem *item, Rectangle panel
) {

    const int gridRow = (int)config->gridSize.y;
    const int gridCol = (int)config->gridSize.x;

    const int btnSize = config->gridBtnSize;
    const int configThickness = config->gridThickness;
    const int thickness = config->enableGrid ? configThickness : 0;

    int gridW = (gridCol * btnSize) + (gridCol + 1) * thickness;
    int gridH = (gridRow * btnSize) + (gridRow + 1) * thickness;

    int panelCenterX = panel.x + panel.width * 0.5f;
    int panelCenterY = panel.y + panel.height * 0.5f;

    int gridX = panelCenterX - gridW * 0.5f;
    int gridY = panelCenterY - gridH * 0.5f;

    Rectangle outlineRect = {
        gridX,
        gridY,
        gridW,
        gridH,
    };

    if (config->enableGrid) {
        DrawRectangleRec(outlineRect, config->gridColor);
    }

    for (int row = 0; row < gridRow; row++) {
        for (int col = 0; col < gridCol; col++) {
            int btnX = (gridX + thickness) + (col * (btnSize + thickness));
            int btnY = (gridY + thickness) + (row * (btnSize + thickness));
            Rectangle btnRect = {btnX, btnY, btnSize, btnSize};

            bool isFlipped = FontItemGetFlip(item, col, row);
            if (isButtonClicked(btnRect)) {
                isFlipped = FontItemFlipBit(item, col, row);
            }

            if (isButtonHovered(btnRect)) {
                state->hoverPosX = col;
                state->hoverPosY = row;
            }

            Color btnColor =
                isFlipped ? config->canvasFillColor : config->canvasCellColor;

            DrawRectangleRec(btnRect, btnColor);
        }
    }

    const int outlineThickness = (configThickness > 0 ? configThickness : 2);

    DrawRectangleLinesEx(outlineRect, outlineThickness, config->gridColor);
}

void Canvas(CanvasState *state, const UiConfig *config, FontItem *item) {

    int winWidth = GetScreenWidth();
    int winHeight = GetScreenHeight();
    int xpos =
        config->itemListWidth + (ITEM_PANEL_MARGIN * 2) + CANVAS_PANEL_MARGIN;
    int ypos = config->toolbarHeight + CANVAS_PANEL_MARGIN;

    int width = winWidth - xpos - CANVAS_PANEL_MARGIN;
    int height = winHeight - config->toolbarHeight - config->statusbarHeight -
                 CANVAS_PANEL_MARGIN * 2;
    Rectangle panelRect = {xpos, ypos, width, height};

    canvasDrawArea(state, config, item, panelRect);
    GuiGroupBox(panelRect, "~canvas");
}
