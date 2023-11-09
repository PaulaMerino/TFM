#include <windows.h>
#include <stdio.h>

// NO FUNIONAAAAAAAAAAAAAAAAAAAAAAAAAAA

int main(int argc, char * argv[]) {
    HANDLE hToken;
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;

    wchar_t cmdline[] = L"C:\\Windows\\System32\\calc.exe";
    wchar_t username[] = L"paula"; // reemplaza esto con el nombre de usuario
    wchar_t password[] = L"Password1!"; // reemplaza esto con la contraseña
    wchar_t domain[] = L"."; // usa "." para el dominio local

    ZeroMemory(&si, sizeof(STARTUPINFOW));
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    si.cb = sizeof(STARTUPINFOW);

    int debug = 1;

    // Crea un nuevo token de acceso con LogonUser
    if (!LogonUserW(username, domain, password, LOGON32_LOGON_INTERACTIVE, LOGON32_PROVIDER_DEFAULT, &hToken)) {
        if (debug) {printf("LogonUser failed with error %d\n", GetLastError());}
        return EXIT_FAILURE;
    }

    // Crea un nuevo proceso y su hilo principal con el nuevo token
    if (!CreateProcessWithTokenW(hToken, LOGON_WITH_PROFILE, NULL, cmdline, 0, NULL, NULL, &si, &pi)) {
        if (debug) {printf("CreateProcessWithTokenW failed with error %d\n", GetLastError());}
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
