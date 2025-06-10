#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// tinyfiledialogs need to below these system headers
// to get wchar_t definition on windows

#include "include/ext/raylib.h"
#include "include/ext/tinyfiledialogs/tinyfiledialogs.h"
#include "include/filedialog.h"

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

void MessageDialog(const char *title, const char *msg) {
    tinyfd_messageBox(title, msg, "close", "error", 0);
}
