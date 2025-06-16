#ifndef FONTITEM_H
#define FONTITEM_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    char *name;
    uint8_t bits[8];
    int value;
    int listIndex;

} FontItem;

FontItem *NewFontItem(const char *name);
void FreeFontItem(FontItem *fi);
void SetFontValue(FontItem *fi, int value);
void SetFontName(FontItem *fi, const char *name);
bool FontItemFlipBit(FontItem *fi, uint8_t col, uint8_t row);
bool FontItemGetFlip(const FontItem *fi, uint8_t col, uint8_t row);
uint8_t *GetFlippedIndexes(const FontItem *fi, int *length);
void LogFontItem(const FontItem *fi);
void LogBinaryFontItem(const FontItem *fi);

#endif
