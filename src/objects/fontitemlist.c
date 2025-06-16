#include "../include/objects/fontitemlist.h"
#include "../include/balloc.h"
#include "../include/ext/raylib.h"
#include "../include/objects/fontitem.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/// Font Item List Functions

FontItemList *NewFontItemList() {
    FontItemList *fl = (FontItemList *)balloc(sizeof(FontItemList));
    if (fl == NULL) {
        TraceLog(
            LOG_ERROR, "Font Item List allocation failed while creating struct"
        );
        return NULL;
    }
    fl->len = 0;
    fl->cap = 10;

    fl->items = balloc(fl->cap * sizeof(FontItem *));

    if (fl->items == NULL) {
        TraceLog(
            LOG_ERROR,
            "Font item List allocation failed while creating item list"
        );

        bfree(fl);
        return NULL;
    }

    fl->names = balloc(fl->cap * sizeof(const char **));

    if (fl->names == NULL) {
        TraceLog(
            LOG_ERROR,
            "Font Item List allocation failed while creating names list"
        );
        bfree(fl->items);
        bfree(fl);
        return NULL;
    }

    return fl;
}

bool resizeFontItemList(FontItemList *fl) {
    if (fl->len >= fl->cap) {
        fl->cap *= 2;
        FontItem **newItems = brealloc(fl->items, fl->cap * sizeof(FontItem *));
        if (newItems == NULL) {
            TraceLog(LOG_ERROR, "Failed to increase size of font item list");
            return false;
        }
        const char **newNames =
            brealloc(fl->names, fl->cap * sizeof(const char *));
        if (newNames == NULL) {
            TraceLog(
                LOG_ERROR, "Failed to increase size of font item name list"
            );
            return false;
        }
        fl->items = newItems;
        fl->names = newNames;
    }

    return true;
}

bool AddToFontItemList(FontItemList *fl, FontItem *item) {
    if (!resizeFontItemList(fl)) {
        return false;
    }

    fl->items[fl->len] = item;
    fl->names[fl->len] = item->name;
    item->listIndex = fl->len;
    fl->len++;

    return true;
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

    bfree(fl->items);
    bfree(fl->names);
    bfree(fl);
}
