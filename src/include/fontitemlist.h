#ifndef FONTITEM_LIST_H
#define FONTITEM_LIST_H

#include "fontitem.h"
#include <stddef.h>

typedef struct {
    FontItem **items;
    const char **names;
    size_t len;
    size_t cap;
} FontItemList;

FontItemList *NewFontItemList();
void FreeFontItemList(FontItemList *fl);
void AddToFontItemList(FontItemList *fl, FontItem *item);
void IndexFontItemList(FontItemList *fl, const char *name);
void IndexFromCodeFontItemList(FontItemList *fl, int code);
void MoveFontItemList(FontItemList *fl, size_t from, size_t to);

#endif
