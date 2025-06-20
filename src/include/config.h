#ifndef CONFIG_H
#define CONFIG_H

#include "uiconfig.h"
#include <stdbool.h>
bool SaveConfigToFile(const UiConfig *config, const char *filepath);
bool LoadConfigFromFile(UiConfig *config, const char *filepath);

#endif
