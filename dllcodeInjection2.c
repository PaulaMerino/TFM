
#include <windows.h>
#include <stdio.h>


int main(int argc, char* argv[]) {

	/*------[SETUP SOME VARIABLES]------*/
	DWORD       TID               = NULL;
	DWORD       PID               = NULL;
	LPVOID      rBuffer           = NULL;
	HANDLE      hProcess          = NULL;
	HANDLE      hThread           = NULL;
	HMODULE     hKernel32         = NULL;
	wchar_t     dllPath[MAX_PATH] = L"C:\\Users\\paula\\dllExampleInjection.dll";
	size_t      pathSize          = sizeof(dllPath);
	size_t      bytesWritten      = 0;

	/*------[GET HANDLE TO PROCESS]------*/
	if (argc < 2) {
		return EXIT_FAILURE;
	}

	PID = atoi(argv[1]);
	hProcess = OpenProcess((PROCESS_VM_OPERATION | PROCESS_VM_WRITE), FALSE, PID);

	if (hProcess == NULL) {
		return EXIT_FAILURE;
	}

	/*------[GET HANDLE TO KERNEL32]------*/
	hKernel32 = GetModuleHandleW(L"kernel32");

	if (hKernel32 == NULL) {
		return EXIT_FAILURE;
	}

	/*------[GET ADDR OF LOADLIBRARY]------*/
	LPTHREAD_START_ROUTINE kawLoadLibrary = (LPTHREAD_START_ROUTINE)GetProcAddress(hKernel32, "LoadLibraryW");

	/*------[ALLOCATE A BUFFER]------*/
	rBuffer = VirtualAllocEx(hProcess, NULL, pathSize, (MEM_COMMIT | MEM_RESERVE), PAGE_READWRITE);

	if (rBuffer == NULL) {
		CloseHandle(hProcess);
	}

	/*------[WRITE TO MEMORY]------*/
	WriteProcessMemory(hProcess, rBuffer, dllPath, pathSize, &bytesWritten);

	/*------[CREATE A THREAD]------*/
	hThread = CreateRemoteThread(hProcess, NULL, 0, kawLoadLibrary, rBuffer, 0, &TID);

	if (hThread == NULL) {
		CloseHandle(hProcess);
	}

	/*------[CLEANLY EXIT]------*/
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	CloseHandle(hProcess);


	return EXIT_SUCCESS;

}