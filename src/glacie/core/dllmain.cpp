/**
 * @file DLLMain.cpp
 * @note DO NOT modify or remove this file!
 */
#include "windows.h"

HMODULE t_hModule = NULL;

void PluginInit();

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        t_hModule = hModule;
        PluginInit();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
