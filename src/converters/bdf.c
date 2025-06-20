#include "../external/raylib.h"
#include "../include/converter.h"
#include <stdio.h>

#define TEMP_FILE "/home/palash/work/mcu/cc/exp2/tempfont.bdf"

bool ExportToBDF(GlyphObj *g, const char *filename) {

    FILE *fptr;

    fptr = fopen(TEMP_FILE, "w");

    if (fptr == NULL) {
        TraceLog(LOG_WARNING, "Failed to open file");
        return false;
    }

    int len = g->count;

    fprintf(fptr, "STARTFONT 2.1\n");
    fprintf(fptr, "FONT %s\n", g->name);
    fprintf(fptr, "SIZE 8 75 75\n");
    fprintf(fptr, "FONTBOUNDINGBOX 8 8 0 0\n");
    fprintf(fptr, "CHARS %d\n", len);

    for (int i = 0; i < len; i++) {
        fprintf(fptr, "STARTCHAR C%03d\n", i);
        fprintf(fptr, "ENCODING %d\n", i);
        fprintf(fptr, "SWIDTH 500 0\n");
        fprintf(fptr, "DWIDTH 8 0\n");
        fprintf(fptr, "BBX 8 8 0 0\n");
        fprintf(fptr, "BITMAP\n");

        GlyphItem *item = g->glyphs[i];
        for (int j = 0; j < 8; j++) {
            int b = item->bits[j];
            fprintf(fptr, "%02X\n", b);
        }

        fprintf(fptr, "ENDCHAR\n");
    }

    fprintf(fptr, "ENDFONT\n");
    fclose(fptr);

    return true;
}
