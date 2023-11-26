#include <windows.h>
#include <stdio.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
    STARTUPINFOEX si;
    PROCESS_INFORMATION pi;
    SIZE_T attributeSize;
    DWORD pid; // PID of the parent process
    int debug = 1;

    // Check the number of parameters
    if (argc < 2) {
        return EXIT_FAILURE;
    }
    pid = atoi(argv[1]);
    

    ZeroMemory(&si, sizeof(STARTUPINFOEX));
    si.StartupInfo.cb = sizeof(STARTUPINFOEX);

    // Open the parent process with all possible access rights
    HANDLE parentProcessHandle = OpenProcess(MAXIMUM_ALLOWED, false, pid);
    if (parentProcessHandle == NULL) {
        if (debug) { printf("OpenProcess failed with error %d\n", GetLastError()); }
        return EXIT_FAILURE;
    }

    // Initialize the process attribute list
        // This first call to the function is to determine the size of the buffer needed for the attribute list.
    InitializeProcThreadAttributeList(NULL, 1, 0, &attributeSize);
    si.lpAttributeList = (LPPROC_THREAD_ATTRIBUTE_LIST)HeapAlloc(GetProcessHeap(), 0, attributeSize);
    if (si.lpAttributeList == NULL) {
        if (debug) { printf("HeapAlloc failed with error %d\n", GetLastError()); }
        return EXIT_FAILURE;
    }

    // This second call to the function is to initialize the buffer with the attribute list.
    InitializeProcThreadAttributeList(si.lpAttributeList, 1, 0, &attributeSize);
    if (si.lpAttributeList == NULL) {
        if (debug) { printf("InitializeProcThreadAttributeList failed with error %d\n", GetLastError()); }
        return EXIT_FAILURE;
    }

    // Set the PROC_THREAD_ATTRIBUTE_PARENT_PROCESS attribute to the identifier of the parent process
    UpdateProcThreadAttribute(si.lpAttributeList, 0, PROC_THREAD_ATTRIBUTE_PARENT_PROCESS, &parentProcessHandle, sizeof(HANDLE), NULL, NULL);
    if (si.lpAttributeList == NULL) {
        if (debug) { printf("UpdateProcThreadAttribute failed with error %d\n", GetLastError()); }
        return EXIT_FAILURE;
    }

    // Create the new process
    CreateProcessA(NULL, (LPSTR)"calc.exe", NULL, NULL, FALSE, EXTENDED_STARTUPINFO_PRESENT, NULL, NULL, &si.StartupInfo, &pi);
    if (pi.hProcess == NULL) {
        if (debug) { printf("CreateProcessW failed with error %d\n", GetLastError()); }
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


/*

1. Abre el proceso indicado en el PID (en este caso, 6200) con OpenProcess.
2. Determina el tamaño del búfer necesario para la lista de atributos con InitializeProcThreadAttributeList.
3. Asigna memoria para la lista de atributos con HeapAlloc y luego inicializa la lista de atributos con InitializeProcThreadAttributeList.
4. Actualiza la lista de atributos para incluir el atributo PROC_THREAD_ATTRIBUTE_PARENT_PROCESS, que especifica el proceso padre del nuevo proceso, con UpdateProcThreadAttribute.
5. Crea un nuevo proceso (en este caso, Notepad) con la lista de atributos especificada usando CreateProcessA.

*/