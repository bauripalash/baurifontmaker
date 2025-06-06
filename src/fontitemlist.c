#include "include/fontitemlist.h"
#include "include/fontitem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// Font Item List Functions

FontItemList *NewFontItemList() {
    FontItemList *fl = (FontItemList *)malloc(sizeof(FontItemList));
    // TODO: Error check
    fl->len = 0;
    fl->cap = 10;

    fl->items = malloc(fl->cap * sizeof(FontItem *));
    fl->names = malloc(fl->cap * sizeof(const char **));

    // TODO: Memory allocation error check

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
    fl->len++;
}

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

    fl->items[to] = tempItem;
    fl->names[to] = tempName;
}

void FreeFontItemList(FontItemList *fl) {
    // TODO: check if fl is valid
    for (size_t i = 0; i < fl->len; i++) {
        FreeFontItem(fl->items[i]);
    }

    free(fl->items);
    free(fl->names);
    free(fl);
}
