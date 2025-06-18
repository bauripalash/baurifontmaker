#include "../include/objects/glyphitem.h"
#include "../include/ext/raylib.h"

#include "../include/balloc.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

GlyphItem *NewGlyphItem(const char *name) {
    GlyphItem *fi = (GlyphItem *)bcalloc(1, sizeof(GlyphItem));
    if (fi == NULL) {
        TraceLog(LOG_ERROR, "Failed to allocate memory for new Font Item");
        return NULL;
    }
    fi->name = (char *)balloc((strlen(name) + 1) * sizeof(char));

    if (fi->name == NULL) {
        TraceLog(LOG_ERROR, "Failed to allocate memory for font item's name");
        bfree(fi);
        return NULL;
    }

    TextCopy(fi->name, name);
    fi->value = 0x0;
    fi->listIndex = 0;

    return fi;
}
void FreeGlyphItem(GlyphItem *fi) {
    if (fi == NULL) {
        return;
    }
    if (fi->name != NULL) {
        bfree(fi->name);
    }
    bfree(fi);
}
void SetGlyphValue(GlyphItem *fi, int value) { fi->value = value; }
void SetGlyphItemName(GlyphItem *gi, const char *name) {
    if (gi == NULL) {
        return;
    }

    if (gi->name == NULL) {
        return;
    }

    if (name == NULL) {
        return;
    }
}

bool GlyphItemFlipBit(GlyphItem *fi, uint8_t col, uint8_t row) {
    uint8_t val = fi->bits[row];
    fi->bits[row] = val ^ (1 << (7 - col));

    return GlyphItemGetFlip(fi, col, row);
}
bool GlyphItemGetFlip(const GlyphItem *fi, uint8_t col, uint8_t row) {
    uint8_t bts = fi->bits[row];
    uint8_t val = (bts >> (7 - col)) & 1;

    return val == 1;
}

uint8_t *GetFlippedIndexes(const GlyphItem *fi, int *length) {
    uint8_t *result = (uint8_t *)bcalloc(64, sizeof(uint8_t));

    *length = 0;

    if (result == NULL) {
        return NULL;
    }

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {

            if (GlyphItemGetFlip(fi, col, row)) {
                result[*length] = row * 8 + col;
                (*length)++;
            }
        }
    }

    return result;
}

void LogGlyphItem(const GlyphItem *fi) {
    printf("[ ");
    for (int i = 0; i < 8; i++) {
        printf("0x%X, ", fi->bits[i]);
    }
    printf(" ]\n");
}

void LogBinaryGlyphItem(const GlyphItem *fi) {
    printf("[\n");
    for (int i = 0; i < 8; i++) {

        for (int b = 7; b >= 0; b--) {
            printf("%d", (fi->bits[i] >> b) & 1);
        }

        printf("\n");
    }

    printf("]\n");
}
