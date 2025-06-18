#include "include/saver.h"
#include "include/ext/raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define TEMP_FILE "/home/palash/work/mcu/cc/exp2/tempfont.bfont"

bool SaveFontFileAsBfont(const FontItemList *list, const char *filename) {
    bool result = true;
    const char *fullpath;
    if (filename != NULL) {
        fullpath = filename;
    } else {
        fullpath = TEMP_FILE;
    }
    const char *dirpath = GetDirectoryPath(fullpath);

    if (!DirectoryExists(dirpath)) {
        if (!MakeDirectory(dirpath)) {
            TraceLog(
                LOG_ERROR, "Failed to create directories for save file : %s",
                dirpath
            );
            return false;
        }
    }

    if (!FileExists(fullpath)) {
        if (!SaveFileText(fullpath, "TEMP FILE")) {
            TraceLog(
                LOG_ERROR, "Failed to create file for saving: '%s'", fullpath
            );
            return false;
        }
    }

    FILE *fptr;

    fptr = fopen(fullpath, "w");

    if (fptr == NULL) {
        TraceLog(LOG_ERROR, "Failed to open file : %s", fullpath);
        return false;
    }

    fprintf(fptr, "# Example Font File Structure <In Progess>\n");
    fprintf(fptr, "# Structure Start ==> \n");
    fprintf(
        fptr, "# <FONT ITEM VALUE>\n# <FONT ITEM NAME>\n# <FONT ITEM BIT "
              "VALUES>\t\t# Each Row is 2 digits\n"
    );
    fprintf(fptr, "# <== Structure End \n");

    for (int i = 0; i < list->len; i++) {
        fprintf(fptr, "%d\n%s\n", list->items[i]->value, list->names[i]);
        for (int j = 0; j < 8; j++) {
            fprintf(fptr, "%d", list->items[i]->bits[j]);
        }
        fprintf(fptr, "\n");
    }

    fclose(fptr);

    return result;
}
