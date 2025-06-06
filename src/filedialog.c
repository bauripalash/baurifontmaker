#include "include/filedialog.h"
#include "include/ext/raylib.h"
#include "include/ext/tinyfiledialogs/tinyfiledialogs.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

bool OpenFileDialog(
    const char *title, char *filename, const char *filters, const char *msg
) {
    bool result = false;

    int numFilters = 0;
    const char **filterList =
        (const char **)TextSplit(filters, ';', &numFilters);

    char *temp =
        tinyfd_openFileDialog(title, filename, numFilters, filterList, msg, 1);

    if (temp != NULL) {
        strcpy(filename, temp);
        result = true;
    }

    return result;
}
bool SaveFileDialog(
    const char *title, char *filename, const char *filters, const char *msg
);
