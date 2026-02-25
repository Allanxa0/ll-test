#include "glacie/core/internal/Logger.h"
#include "glacie/core/version.h"
#include <windows.h>

extern HMODULE t_hModule;

void PluginInit() {
    Info("  _____ _            _    ");
    Info(" / ____| |          (_)  ");
    Info("| |  __| | __ _  ___ _  ___     \033[38;5;221mVersion:{}", PLUGIN_VERSION_STRING);
    Info("| | |_ | |/ _` |/ __| |/ _ \\    \033[38;5;218mGithub:{}", "https://github.com/GlacieTeam");
    Info("| |__| | | (_| | (__| |  __/");
    Info(" \\_____|_|\\__,_|\\___|_|\\___|");
    Info("");
}
