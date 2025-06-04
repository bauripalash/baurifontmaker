#ifndef FONTITEM_H
#define FONTITEM_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
typedef struct {
    char *name;
    uint8_t bits[8];
    int nameValue;
    int listIndex;

} FontItem;

FontItem *NewFontItem(const char *name);
void FreeFontItem(FontItem *fi);
void SetNameValue(FontItem *fi, int value);
uint8_t FontItemFlipBit(FontItem *fi, uint8_t col, uint8_t row);
bool FontItemGetFlip(FontItem *fi, uint8_t col, uint8_t row);
uint8_t *GetFlippedIndexes(FontItem *fi, int *length);
void LogFontItem(FontItem *fi);
void LogBinaryFontItem(FontItem *fi);

typedef struct {
    FontItem **items;
    size_t len;
    size_t cap;
} FontItemList;

FontItemList *NewFontItemList();
void FreeFontItemList(FontItemList *fl);
void AddToFontItemList(FontItemList *fl, FontItem *item);

#endif
