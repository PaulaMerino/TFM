#include <windows.h>

BOOL __stdcall DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpvReserved) {

    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
        CreateProcess(TEXT("calc.exe"),NULL,NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi);

        CloseHandle(pi.hProcess);
	    CloseHandle(pi.hThread);

        break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;

}