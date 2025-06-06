#ifndef _STRLIST_H
#define _STRLIST_H

#include <stdbool.h>
#include <stddef.h>
#define MAX_STRING_LEN 20
#define MAX_ITEMS      255
#define INIT_ITEMS_LEN 10

typedef struct {
    const char **items;
    size_t len;
    size_t cap;
} StrList;

StrList *NewStrList();
void FreeStrList(StrList *l);
int AddStrList(StrList *l, const char *item);
void RemoveIndexStrList(StrList *l, size_t index);
void RemoveValStrList(StrList *l, const char *value);
bool HasItemStrList(StrList *l, const char *value);
int GetIndexStrList(StrList *l, const char *value);

#endif
