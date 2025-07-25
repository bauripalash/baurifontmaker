#include "include/gui.h"
#include "external/raylib.h"
#include "include/codegen.h"
#include "include/colors.h"
#include "include/config.h"
#include "include/converter.h"
#include "include/defaults.h"
#include "include/glyph.h"
#include "include/glyphitem.h"
#include "include/saver.h"
#include "include/uiconfig.h"
#include "include/uistates.h"
#include "include/utils.h"
#include "include/widgets/canvas.h"
#include "include/widgets/filedialog.h"
#include "include/widgets/itemselector.h"
#include "include/widgets/toolbar.h"
#include "include/windows/edititem.h"
#include "include/windows/export.h"
#include "include/windows/infoedit.h"
#include "include/windows/newitem.h"
#include "include/windows/settings.h"

#define BALLOC_IMPL
#include "include/balloc.h"
#include "include/themes/theme.h"

#define RAYGUI_IMPLEMENTATION
#include "external/raygui.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MARGIN 20 // Canvas Margin

bool ShowPrelimErrorMsg(GuiPrelimError err);
bool DrawWindow(Gui *ui, GuiPrelimError *err);
void Layout(Gui *ui);
void StatusBarLayout(Gui *ui);
bool handleAppError(const Gui *ui);

Gui *NewGUI(GuiPrelimError *err) {
    Gui *ui = (Gui *)balloc(sizeof(Gui));
    if (ui == NULL) {
        *err = PRELIM_GUI_ALLOC_FAILED;
        return NULL;
    }
    ui->states = NULL;
    ui->glyph = NULL;
    ui->conf = NewUiConfig();

    if (ui->conf == NULL) {
        FreeGui(ui);
        *err = PRELIM_CONFIG_ALLOC_FAILED;
        return NULL;
    }

    ui->winWidth = 800;
    ui->winHeight = 640;
    ui->title = TextFormat("BauriGlyphMaker");
    ui->itemListAnchor = (Vector2){.x = 0, .y = 0};
    ui->currentItem = NULL;

    strcpy(ui->openFilename, "");
    strcpy(ui->saveFilename, "");

    ui->glyph = NewGlyph(NULL);

    if (ui->glyph == NULL) {
        FreeGui(ui);
        *err = PRELIM_GLYPHOBJ_ALLOC_FAILED;
        return NULL;
    }

    bool isok = AddNewGlyphItem(ui->glyph, "0x0000", 0x0);

    if (!isok) {
        FreeGui(ui);
        *err = PRELIM_GLYPHITEM_ALLOC_FAILED;
        return NULL;
    }

    ui->currentItem = ui->glyph->glyphs[0];
    ui->apperr = APPERR_OK;

    *err = PRELIM_OK;

    return ui;
}

void FreeGui(Gui *ui) {
    if (ui == NULL) {
        return;
    }
    if (ui->glyph != NULL) {
        FreeGlyph(ui->glyph);
    }
    if (ui->conf != NULL) {
        FreeUiConfig(ui->conf);
    }

    if (ui->states != NULL) {
        FreeUiStates(ui->states);
    }
    bfree(ui);
}

void UpdateData(Gui *ui) {
    int h = GetScreenHeight();
    int w = GetScreenWidth();
    ui->winHeight = h;
    ui->winWidth = w;
}

void FreeStyles() {
    Font f = GuiGetFont();
    UnloadFont(f);
}

bool ShowPrelimErrorMsg(GuiPrelimError err) {
    char errMSg[255];

    switch (err) {
    case PRELIM_OK:
        break;
    case PRELIM_GUI_ALLOC_FAILED: {
        strcpy(errMSg, "GUI Structure");
        break;
    }

    case PRELIM_CONFIG_ALLOC_FAILED: {
        strcpy(errMSg, "Application Configuration");
        break;
    }

    case PRELIM_GLYPHOBJ_ALLOC_FAILED: {
        strcpy(errMSg, "Glyph Object");
        break;
    }

    case PRELIM_GLYPHITEM_ALLOC_FAILED: {
        strcpy(errMSg, "Default Glyph Item");
        break;
    }

    case PRELIM_STATES_ALLOC_FAILED: {
        strcpy(errMSg, "GUI Application States");
        break;
    }
    }

    bool clicked =
        GuiMessageBox(
            GetCenteredRect(DEF_ERR_WIN_WIDTH, DEF_ERR_WIN_HEIGHT),
            GuiIconText(ICON_WARNING, "Fatal Memory Error"),
            TextFormat("Failed to allocate memory for %s", errMSg), "Close"
        ) != -1;
    return clicked;
}

void clearAppError(Gui *ui) { ui->apperr = APPERR_OK; }

bool hasAppError(const Gui *ui) { return ui->apperr != APPERR_OK; }

void setupStates(Gui *ui) {
    if (ui->states == NULL || ui->conf == NULL) {
        return;
    }

    ui->states->settings.enableGrid = ui->conf->enableGrid;
}

void handleConfig(Gui *ui) { bool isok = LoadConfigFromFile(ui->conf, NULL); }

void GuiMain() {
    SeedRandom();

    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(DEF_WIN_WIDTH, DEF_WIN_HEIGHT, DEF_WIN_TITLE);
    SetWindowMinSize(DEF_WIN_WIDTH, DEF_WIN_HEIGHT);
    SetAppIcon();
    LoadAppFont();
    LoadAppLightTheme();
    GuiPrelimError err;

    Gui *ui = NewGUI(&err);

    SetTargetFPS(60);

    if (ui != NULL) {
        ui->states = NewUiStates();

        if (ui->states == NULL) {
            if (err == PRELIM_OK) {
                err = PRELIM_STATES_ALLOC_FAILED;
            }
        }

        setupStates(ui);
    }

    handleConfig(ui);

    while (!WindowShouldClose()) {
        BeginDrawing();
        {
            ClearBackground(ColorBackground);
            if (err != PRELIM_OK) {
                bool clicked = ShowPrelimErrorMsg(err);
                EndDrawing();
                if (clicked) {
                    break;
                }
                continue;
            }
            UpdateData(ui);
            Layout(ui);
        }
        EndDrawing();
    }

#ifdef BALLOC_DEBUG
    MemStats();
#endif
    FreeGui(ui);
    FreeStyles();
    CloseWindow();
}

bool DrawWindow(Gui *ui, GuiPrelimError *err) {
    BeginDrawing();
    {
        ClearBackground(ColorBackground);
        if (err != PRELIM_OK) {
            bool clicked = ShowPrelimErrorMsg(*err);
            EndDrawing();

            if (clicked) {
                return false;
            }
        }
        UpdateData(ui);
        Layout(ui);
    }
    EndDrawing();
    return true;
}

void handleNewItemWindow(Gui *ui) {
    if (ui->states->itemSelector.newBtnClicked) {
        ui->states->newItem.windowActive = true;
        CleanNewItemState(&ui->states->newItem);
    }
    if (NewItemWindow(&ui->states->newItem)) {
        const char *name = ui->states->newItem.nameStr;
        int value = ui->states->newItem.hexValue;
        if (!AddNewGlyphItem(ui->glyph, name, value)) {
            ui->apperr = APPERR_NEWITEM_ALLOC;
            return;
        }
    }
}

void handleEditItemWindow(Gui *ui) {
    if (ui->states->itemSelector.editBtnClicked) {
        ui->states->editItem.windowActive = true;
        SetStateEditItem(
            &ui->states->editItem, ui->currentItem, ui->glyph->count
        );
    }

    int result = EditItemWindow(
        &ui->states->editItem, ui->currentItem, ui->glyph->count
    );

    if (result == EDIT_SAVE_CLICK) {
        TraceLog(ICON_WARNING, "Clicked Save");
        if (!TextIsEqual(ui->states->editItem.nameStr, ui->currentItem->name)) {
            TraceLog(
                ICON_WARNING, "Name is Not Equal : old:%s | new:%s",
                ui->currentItem->name, ui->states->editItem.nameStr
            );
            RenameGlyphItem(
                ui->glyph, ui->currentItem->listIndex,
                ui->states->editItem.nameStr
            );
        }
    } else if (result == EDIT_REMOVE_CLICK) {

        TraceLog(ICON_WARNING, "Clicked Remove");
    } else {
        return;
    }
}

void handleItemSelector(Gui *ui) {

    ui->currentItem = ui->glyph->glyphs[ui->states->itemSelector.active];
}

void updateConfigFromSettings(Gui *ui) {
    ui->conf->gridSize.y = ui->states->settings.canvasRow;
    ui->conf->gridSize.x = ui->states->settings.canvasColumn;
    ui->conf->enableGrid = ui->states->settings.enableGrid;
}
void handleSettingsDialog(Gui *ui) {
    if (ui->states->settings.windowActive) {
        bool result = SettingsWindow(&ui->states->settings);
        if (result) {
            LogSettings(&ui->states->settings);
            updateConfigFromSettings(ui);
            SaveConfigToFile(ui->conf, NULL);
        }
    }
}

void handleInfoEditWindow(Gui *ui) {
    if (ui->states->infoEdit.windowActive) {
        bool result = InfoEditWindow(&ui->states->infoEdit, ui->glyph);
    }
}

void fillExportOutput(Gui *ui, bool save, bool copy) {
    UiStates *states = ui->states;
    int type = states->export.typeSelectActive;
    int subtype = -1;
    bool saveResult = false;
    char *saveFilename = NULL;

    if (save) {
        saveFilename = (char *)bcalloc(512, sizeof(char));
        if (saveFilename == NULL) {
            TraceLog(LOG_ERROR, "Failed to save file");
            return;
        }
    }

    ClearExportBuffer(&states->export);
    if (type == TYPE_SEL_CODE) {
        subtype = states->export.langSelectActive;
        switch (subtype) {
        case LANG_SEL_CHEADER: {
            states->export.buffer = GenerateCHeader(ui->glyph);
            if (save) {
                saveResult = SaveFileDialog(
                    "Export File to C Header", saveFilename, "*.h;*.c",
                    "C Header (*.h)"
                );
                if (saveResult) {
                    if (!SaveFileText(saveFilename, states->export.buffer)) {
                        TraceLog(LOG_ERROR, "Failed to save C Header");
                        // Add Error handling with gui
                    }
                }
            }

            break;
        }

        case LANG_SEL_PYTHON: {
            states->export.buffer = GeneratePython(ui->glyph);
            if (save) {
                saveResult = SaveFileDialog(
                    "Export File to Python Code", saveFilename, "*.py",
                    "Python Code (*.py)"
                );

                if (saveResult) {
                    if (!SaveFileText(saveFilename, states->export.buffer)) {
                        TraceLog(LOG_ERROR, "Failed to save Python Code");
                    }
                }
            }
            break;
        }

        case LANG_SEL_RUST: {
            states->export.buffer = GenerateRust(ui->glyph);
            // TraceLog(LOG_WARNING, "Rust Code Export Not Yet Available");
            break;
        }
        }

        if (copy) {
            SetClipboardText(states->export.buffer);
        }
    } else if (type == TYPE_SEL_FONT) {
        subtype = states->export.fontTypeActive;
        switch (subtype) {
        case FONT_SEL_BDF: {
            states->export.buffer = GenerateBDFFont(ui->glyph);
            if (copy) {
                SetClipboardText(states->export.buffer);
            }
            break;
        }

        case FONT_SEL_TTF: {
            // TraceLog(LOG_WARNING, "TTF Font Export Not Yet Available");
            break;
        }

        case FONT_SEL_OTF: {
            // TraceLog(LOG_WARNING, "OTF Font Export Not Yet Available");
            break;
        }
        }
    }

    if (saveFilename != NULL) {
        bfree(saveFilename);
    }
}

void handleExportWindow(Gui *ui) {

    if (ui->states->toolbar.exportBtnClicked) {
        ui->states->export.windowActive = true;
        ui->states->toolbar.exportBtnClicked = false;
    }
    if (ui->states->export.windowActive) {
        bool result = ExportWindow(&ui->states->export, ui->glyph);

        fillExportOutput(
            ui, ui->states->export.saveBtnClicked,
            ui->states->export.copyBtnClicked
        );

        if (ui->states->export.saveBtnClicked) {
            ui->states->export.saveBtnClicked = false;
            ui->states->export.windowActive = false;
        }

        if (ui->states->export.copyBtnClicked) {
            ui->states->export.copyBtnClicked = false;
        }
    } else {
        ClearExportBuffer(&ui->states->export);
    }
}

void handleOpenFileDialog(Gui *ui) {
    bool ok = OpenFileDialog(
        "Open Font File", ui->openFilename, "*.bgmf;*.baufnt;*.txt",
        "BauriGlyphMaker Font Files (*.bfont)"
    );

    if (ok) {
        printf("Open File -> %s\n", ui->openFilename);
    }
}

void updateConfigToSettings(Gui *ui) {
    ui->states->settings.canvasRow = (int)ui->conf->gridSize.y;
    ui->states->settings.canvasColumn = (int)ui->conf->gridSize.x;
    ui->states->settings.enableGrid = ui->conf->enableGrid;
}

void handleToolbar(Gui *ui) {
    if (ui->states->toolbar.openBtnClicked) {
        handleOpenFileDialog(ui);
        ui->states->toolbar.openBtnClicked = false;
    }

    if (ui->states->toolbar.settingsBtnClicked) {
        updateConfigToSettings(ui);
        ui->states->settings.windowActive = true;
        ui->states->toolbar.settingsBtnClicked = false;
    }

    if (ui->states->toolbar.saveBtnClicked) {
        bool isok = SaveFontFileAsBfont(ui->glyph, NULL);
        if (!isok) {
            TraceLog(LOG_ERROR, "Failed to save file");
        }

        ui->states->toolbar.saveBtnClicked = false;
    }

    if (ui->states->toolbar.glyphOptBtnClicked) {
        UpdateInfo(&ui->states->infoEdit, ui->glyph);
        ui->states->infoEdit.windowActive = true;
        ui->states->toolbar.glyphOptBtnClicked = false;
    }

    handleSettingsDialog(ui);
    handleInfoEditWindow(ui);
    handleExportWindow(ui);
}

void FillScreenForPopup(const Gui *ui) {
    DrawRectangleRec(
        (Rectangle){
            0,
            0,
            GetScreenWidth(),
            GetScreenHeight(),
        },
        Fade(ColorGrayDarkest, 0.5f)
    );
}

void Layout(Gui *ui) {
    GuiEnableTooltip();
    bool popup = WindowPopupActive(ui->states);

    ui->conf->isPopupActive = popup;

    if (popup || hasAppError(ui)) {
        GuiLock();
    }

    GuiPanel((Rectangle){0, 0, ui->winWidth, ui->winHeight}, NULL);
    Toolbar(&ui->states->toolbar);
    ItemSelector(&ui->states->itemSelector, ui->glyph);

    handleItemSelector(ui);
    StatusBarLayout(ui);

    Canvas(&ui->states->canvas, ui->conf, ui->currentItem);

    GuiUnlock();

    if (popup || hasAppError(ui)) {

        FillScreenForPopup(ui);
    }

    handleNewItemWindow(ui);
    handleEditItemWindow(ui);
    handleToolbar(ui);

    if (hasAppError(ui)) {
        if (handleAppError(ui)) {
            clearAppError(ui);
        }
    }
}

bool handleAppError(const Gui *ui) {
    // TD: make it modular

    char errorMsg[128];
    char errTitle[128] = {0};

    switch (ui->apperr) {
    case APPERR_NEWITEM_ALLOC: {
        strcpy(errorMsg, "Failed to Create New Font Item");
        strcpy(
            errTitle,
            GuiIconText(ICON_WARNING, "Fatal Memory Allocation Error Occured")
        );
        break;
    }
    case APPERR_FONTLIST_ADD: {
        strcpy(errorMsg, "Failed to add newly created font to list");
        break;
    }
    default: {
        break;
    }
    }

    if (strlen(errTitle) == 0) {
        strcpy(errTitle, GuiIconText(ICON_WARNING, "Fatal Error Occured"));
    }

    bool result = GuiMessageBox(
                      GetCenteredRect(DEF_ERR_WIN_WIDTH, DEF_ERR_WIN_HEIGHT),
                      errTitle, errorMsg, "Close"
                  ) != -1;

    return result;
}

#define ZOOM_PANEL_WIDTH  200
#define ZOOM_PANEL_HEIGHT 10
#define ZOOM_PANEL_MARGIN 20

void StatusBarLayout(Gui *ui) {
    int hoverIndex = ui->states->itemSelector.focus;
    int hoverPosX = ui->states->canvas.hoverPosX;
    int hoverPosY = ui->states->canvas.hoverPosY;
    GlyphItem *hoveredFontItem = ui->glyph->glyphs[hoverIndex];
    Rectangle statusBarRect = (Rectangle){
        0,
        ui->winHeight - ui->conf->statusbarHeight,
        ui->winWidth,
        ui->conf->statusbarHeight,
    };
    GuiStatusBar(
        statusBarRect, TextFormat(
                           "[%d, %d] | %s (0x%x)", hoverPosX, hoverPosY,
                           hoveredFontItem->name, hoveredFontItem->value
                       )
    );

    GuiSliderBar(
        (Rectangle){
            ui->winWidth - ZOOM_PANEL_WIDTH - ZOOM_PANEL_MARGIN,
            statusBarRect.y +
                ((ui->conf->statusbarHeight - ZOOM_PANEL_HEIGHT) * 0.5f),
            ZOOM_PANEL_WIDTH,
            ZOOM_PANEL_HEIGHT,

        },
        GuiIconText(
            ICON_LENS,
            TextFormat("%0.2f%%", ui->states->canvas.zoomValue * 100.0f)
        ),
        NULL, &ui->states->canvas.zoomValue, 0.5f, ui->states->canvas.maxZoom
    );
}
