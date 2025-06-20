#ifndef GLYPH_ITEM_H
#define GLYPH_ITEM_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    char *name;
    uint8_t bits[8];
    int value;
    int listIndex;

} GlyphItem;

GlyphItem *NewGlyphItem(const char *name);
void FreeGlyphItem(GlyphItem *gi);
void SetGlyphValue(GlyphItem *gi, int value);
void SetGlyphItemName(GlyphItem *gi, const char *name);
bool GlyphItemFlipBit(GlyphItem *gi, uint8_t col, uint8_t row);
bool GlyphItemGetFlip(const GlyphItem *gi, uint8_t col, uint8_t row);
uint8_t *GetFlippedIndexes(const GlyphItem *gi, int *length);
void LogGlyphItem(const GlyphItem *gi);
void LogBinaryGlyphItem(const GlyphItem *gi);

#endif
