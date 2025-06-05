#include "include/fontitem.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FontItem *NewFontItem(const char *name) {
    FontItem *fi = (FontItem *)malloc(sizeof(FontItem));
    fi->name = strdup(name);
    fi->nameValue = 0x0;
    fi->listIndex = 0;

    return fi;
}
void FreeFontItem(FontItem *fi) {
    free(fi->name);
    free(fi);
}
void SetNameValue(FontItem *fi, int value) { fi->nameValue = value; }

uint8_t FontItemFlipBit(FontItem *fi, uint8_t col, uint8_t row) {
    uint8_t val = fi->bits[row];
    fi->bits[row] = val ^ (1 << (7 - col));

    return val;
}
bool FontItemGetFlip(FontItem *fi, uint8_t col, uint8_t row) {
    uint8_t bts = fi->bits[row];
    uint8_t val = (bts >> (7 - col)) & 1;

    return val == 1;
}

uint8_t *GetFlippedIndexes(FontItem *fi, int *length) {
    uint8_t *result = (uint8_t *)calloc(64, sizeof(uint8_t));

    *length = 0;

    if (result == NULL) {
        return NULL;
    }

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {

            if (FontItemGetFlip(fi, col, row)) {
                result[*length] = row * 8 + col;
                (*length)++;
            }
        }
    }

    return result;
}

void LogFontItem(FontItem *fi) {
    printf("[ ");
    for (int i = 0; i < 8; i++) {
        printf("0x%X, ", fi->bits[i]);
    }
    printf(" ]\n");
}

void LogBinaryFontItem(FontItem *fi) {
    printf("[\n");
    for (int i = 0; i < 8; i++) {

        for (int b = 7; b >= 0; b--) {
            printf("%d", (fi->bits[i] >> b) & 1);
        }

        printf("\n");
    }

    printf("]\n");
}
