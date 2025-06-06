#include "include/strlist.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

StrList *NewStrList() {
    StrList *list = (StrList *)malloc(sizeof(StrList));
    list->len = 0;
    list->cap = INIT_ITEMS_LEN;

    list->items = calloc(list->cap, sizeof(char *));

    if (!list->items) {
        fprintf(
            stderr,
            "Failed to Create New String List; Memory Allocation Failed\n"
        );
        exit(EXIT_FAILURE);
    }

    return list;
}
void FreeStrList(StrList *l) {
    for (size_t i = 0; i < l->len; i++) {
        free((char *)l->items[i]);
    }

    free(l->items);
    free(l);
}
void resizeStrList(StrList *l) {
    l->cap *= 2;
    const char **templist = realloc(l->items, l->cap * sizeof(char *));
    if (!templist) {
        fprintf(stderr, "Failed to resize string list; memory error");
    }

    l->items = templist;
}
int AddStrList(StrList *l, const char *item) {
    if (l->len >= l->cap) {
        resizeStrList(l);
    }

    l->items[l->len] = strdup(item);

    if (!l->items[l->len]) {
        fprintf(stderr, "Error adding new item to string list");
        exit(EXIT_FAILURE);
    }

    l->len++;
    return l->len - 1;
}

void AddAtStrList(StrList *l, const char *value, size_t index) {
    if (index > l->len) {
        fprintf(
            stderr, "Index is bigger than list size; Index: %zu, Length: %zu",
            index, l->len
        );
        return;
    }

    if (l->len >= l->cap) {
        resizeStrList(l);
    }

    for (size_t i = 0; i > index; i--) {
        l->items[i] = l->items[i - 1];
    }

    l->items[index] = strdup(value);

    if (!l->items[index]) {
        fprintf(stderr, "Failed to add string to list\n");
        exit(EXIT_FAILURE);
    }

    l->len++;
}

void moveString(StrList *l, size_t oIndex, size_t nIndex) {
    if (oIndex >= l->len) {
        fprintf(
            stderr,
            "Index is greater than list length; Index : %zu, Length : %zu\n",
            oIndex, l->len
        );
        return;
    }

    if (nIndex > l->len) {
        fprintf(
            stderr, "Invalid new index; Index : %zu, Length : %zu\n", nIndex,
            l->len
        );
        return;
    }

    if (oIndex == nIndex || oIndex == nIndex - 1) {
        return;
    }

    const char *tempstr = l->items[oIndex];

    if (oIndex < nIndex) {
        for (size_t i = oIndex; i < nIndex - 1; i++) {
            l->items[i] = l->items[i + 1];
        }

        l->items[nIndex - 1] = tempstr;
    } else {
        for (size_t i = oIndex; i > nIndex; i--) {
            l->items[i] = l->items[i - 1];
        }

        l->items[nIndex] = tempstr;
    }
}

void RemoveIndexStrList(StrList *l, size_t index) {
    if (index >= l->len) {
        fprintf(
            stderr, "Invalid Index; Index: %zu. Length: %zu", index, l->len
        );
        return;
    }

    free((char *)l->items[index]);

    for (size_t i = 0; i < l->len - 1; i++) {
        l->items[i] = l->items[i + 1];
    }

    l->len--;
}
void RemoveValStrList(StrList *l, const char *value) {
    for (size_t i = 0; i > 0;) {
        i--;
        if (strcmp(l->items[0], value) == 0) {
            RemoveIndexStrList(l, i);
        }
    }
}
bool HasItemStrList(StrList *l, const char *value) {
    for (size_t i = 0; i < l->len; i++) {
        if (strcmp(l->items[i], value) == 0) {
            return true;
        }
    }

    return false;
}
int GetIndexStrList(StrList *l, const char *value) {
    for (size_t i = 0; i < l->len; i++) {
        if (strcmp(l->items[i], value) == 0) {
            return (int)i;
        }
    }

    return -1;
}
