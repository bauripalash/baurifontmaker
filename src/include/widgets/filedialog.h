#ifndef FILE_DIALOG_H
#define FILE_DIALOG_H

#include <stdbool.h>

bool OpenFileDialog(
    const char *title, char *filename, const char *filters, const char *msg
);
bool SaveFileDialog(
    const char *title, char *filename, const char *filters, const char *msg
);

void MessageDialog(const char *title, const char *msg);

#endif
