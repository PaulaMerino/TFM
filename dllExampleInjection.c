#include <windows.h>

BOOL __stdcall DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpvReserved) {

    switch (dwReason) {

    case DLL_PROCESS_ATTACH:
        MessageBoxW(NULL, L"DLL Injection DONE :>", L"TFM", MB_ICONEXCLAMATION);
        //CreateProcess(NULL, L"calc.exe", NULL, NULL, FALSE, 0, NULL, NULL, NULL, NULL);
        break;

    }

    return TRUE;

}