#include "include/config.h"
#include "include/defaults.h"
#include <stdbool.h>
#define RINI_IMPLEMENTATION
#include "external/raylib.h"
#include "external/rini.h"

#define TEMP_FILE             "/home/palash/work/mcu/cc/exp2/bauriglyphmaker.ini"
#define VERSION_KEY           "VERSION"
#define ENABLE_GRID_KEY       "ENABLE_GRID"
#define BTN_SIZE_KEY          "BTN_SIZE"
#define GRID_THICKNESS_KEY    "GRID_THICKNESS"
#define TOOLBAR_HEIGHT_KEY    "TOOLBAR_HEIGHT"
#define STATUSBAR_HEIGHT_KEY  "STATUSBAR_HEIGHT"
#define ITEMLIST_WIDTH_KEY    "ITEMLIST_WIDTH"
#define GRID_COLUMN_KEY       "GRID_COLUMN"
#define GRID_ROW_KEY          "GRID_ROW"
#define GRID_COLOR_KEY        "GRID_COLOR"
#define CANVAS_CELL_COLOR_KEY "CANVAS_CELL"
#define CANVAS_FILL_COLOR_KEY "CANVAS_FILL"

#define GRID_TRUE             1
#define GRID_FALSE            -1

bool SaveConfigToFile(const UiConfig *config, const char *filepath) {
    const char *fullpath = filepath == NULL ? TEMP_FILE : filepath;
    const char *path = GetDirectoryPath(fullpath);

    if (!IsPathFile(fullpath)) {
        TraceLog(LOG_ERROR, "Config path doesn't points to a filepath");
    }
    rini_data confFile;
    if (!FileExists(fullpath)) {
        MakeDirectory(path);
        confFile = rini_load(NULL);

    } else {
        confFile = rini_load(fullpath);
    }

    // NOTE: `rini_load` is leaking memory <FIX>

    rini_set_comment_line(&confFile, "bauriglyphmaker config");
    rini_set_comment_line(&confFile, NULL);

    rini_set_value_text(
        &confFile, VERSION_KEY, VERSION, "bauriglyphmaker version"
    );
    rini_set_value(
        &confFile, ENABLE_GRID_KEY, config->enableGrid ? GRID_TRUE : GRID_FALSE,
        "Enable Grid"
    );
    rini_set_value(
        &confFile, BTN_SIZE_KEY, config->gridBtnSize, "Canvas button size"
    );

    rini_set_value(
        &confFile, TOOLBAR_HEIGHT_KEY, config->toolbarHeight, "Toolbar Height"
    );
    rini_set_value(
        &confFile, STATUSBAR_HEIGHT_KEY, config->statusbarHeight,
        "Statusbar Height"
    );
    rini_set_value(
        &confFile, ITEMLIST_WIDTH_KEY, config->itemListWidth, "Item list width"
    );
    Vector2 gridSize = config->gridSize;
    rini_set_value(
        &confFile, GRID_COLUMN_KEY, (int)gridSize.x, "Grid Column Size"
    );

    rini_set_value(&confFile, GRID_ROW_KEY, (int)gridSize.y, "Grid Row Size");

    rini_set_value(
        &confFile, GRID_COLOR_KEY, ColorToInt(config->gridColor), "Grid Color"
    );

    rini_set_value(
        &confFile, CANVAS_CELL_COLOR_KEY, ColorToInt(config->canvasCellColor),
        "Canvas Cell Color"
    );
    rini_set_value(
        &confFile, CANVAS_FILL_COLOR_KEY, ColorToInt(config->canvasFillColor),
        "Canvas Fill Color"
    );

    rini_save(confFile, fullpath);

    rini_unload(&confFile);

    return true;
}

bool LoadConfigFromFile(UiConfig *config, const char *filepath) {
    const char *fullpath = filepath == NULL ? TEMP_FILE : filepath;
    rini_data confFile;
    if (FileExists(fullpath)) {
        confFile = rini_load(fullpath);
    } else {

        confFile = rini_load(NULL);
    }

    config->enableGrid =
        rini_get_value(confFile, ENABLE_GRID_KEY) != GRID_FALSE;

    return true;
}
