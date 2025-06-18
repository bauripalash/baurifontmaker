#ifndef CONVERT_BDF_H
#define CONVERT_BDF_H

#include "../objects/fontitemlist.h"
#include <stdbool.h>

bool ExportToBDF(FontItemList *list, const char *filename);

#endif
