#include "include/fontitemlist.h"
#include "include/ext/raylib.h"
#include "include/fontitem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// Font Item List Functions

FontItemList *NewFontItemList() {
    FontItemList *fl = (FontItemList *)malloc(sizeof(FontItemList));
    if (fl == NULL) {
        TraceLog(
            LOG_ERROR, "Font Item List allocation failed while creating struct"
        );
        return NULL;
    }
    fl->len = 0;
    fl->cap = 10;

    fl->items = malloc(fl->cap * sizeof(FontItem *));

    if (fl->items == NULL) {
        TraceLog(
            LOG_ERROR,
            "Font item List allocation failed while creating item list"
        );

        free(fl);
        return NULL;
    }

    fl->names = malloc(fl->cap * sizeof(const char **));

    if (fl->names == NULL) {
        TraceLog(
            LOG_ERROR,
            "Font Item List allocation failed while creating names list"
        );
        free(fl->items);
        free(fl);
        return NULL;
    }

    return fl;
}

void resizeFontItemList(FontItemList *fl) {
    if (fl->len >= fl->cap) {
        fl->cap *= 2;
        FontItem **newItems = realloc(fl->items, fl->cap * sizeof(FontItem *));
        const char **newNames =
            realloc(fl->names, fl->cap * sizeof(const char *));
        // TODO: Error check
        fl->items = newItems;
        fl->names = newNames;
    }
}

void AddToFontItemList(FontItemList *fl, FontItem *item) {
    resizeFontItemList(fl);

    fl->items[fl->len] = item;
    fl->names[fl->len] = item->name;
    item->listIndex = fl->len;
    fl->len++;
}

void IndexFontItemList(FontItemList *fl, const char *name); // TODO:

void IndexFromCodeFontItemList(FontItemList *fl, int code); // TODO:

void RemoveFromFontItem(FontItemList *fl, FontItem *item); // TODO:

void MoveFontItemList(FontItemList *fl, size_t from, size_t to) {
    if (from >= fl->len || to >= fl->len) {
        fprintf(stderr, "Invalid index");
        // TODO: Better error messages

        return;
    }

    if (from == to) {
        return;
    }

    FontItem *tempItem = fl->items[from];
    const char *tempName = fl->names[from];

    if (from < to) {
        memmove(
            &fl->items[from], &fl->items[from + 1],
            (to - from) * sizeof(FontItem *)
        );
        memmove(
            &fl->names[from], &fl->items[from + 1],
            (to - from) * sizeof(FontItem *)
        );
    } else { // from > to
        memmove(
            &fl->items[to + 1], &fl->items[to], (from - to) * sizeof(FontItem *)
        );
        memmove(
            &fl->names[to + 1], &fl->names[to],
            (from - to) * sizeof(const char *)
        );
    }

    tempItem->listIndex = to;
    fl->items[to] = tempItem;
    fl->names[to] = tempName;
}

void FreeFontItemList(FontItemList *fl) {
    if (fl == NULL) {
        return;
    }
    for (size_t i = 0; i < fl->len; i++) {
        FreeFontItem(fl->items[i]);
    }

    free(fl->items);
    free(fl->names);
    free(fl);
}
