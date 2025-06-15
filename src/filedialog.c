#include <stdbool.h>
#include <string.h>

// tinyfiledialogs need to below these system headers
// to get wchar_t definition on windows

#include "include/ext/raylib.h"
#ifndef BFM_OS_WEB
#include "include/ext/tinyfiledialogs/tinyfiledialogs.h"
#endif
#include "include/filedialog.h"

bool OpenFileDialog(
    const char *title, char *filename, const char *filters, const char *msg
) {
#ifndef BFM_OS_WEB
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
#else
    // TODO: messagebox showing drop file to open/load file
    return false;
#endif
}
bool SaveFileDialog(
    const char *title, char *filename, const char *filters, const char *msg
);

void MessageDialog(const char *title, const char *msg) {
#ifndef BFM_OS_WEB
    tinyfd_messageBox(title, msg, "close", "error", 0);
#endif
}
