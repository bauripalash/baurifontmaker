#include <stdbool.h>

bool OpenFileDialog(
    const char *title, char *filename, const char *filters, const char *msg
);
bool SaveFileDialog(
    const char *title, char *filename, const char *filters, const char *msg
);
