#include <Windows.h>
#include "jvm_dumper.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)hookSetup, NULL, 0, NULL);
        break;
    }
    return TRUE;
}

