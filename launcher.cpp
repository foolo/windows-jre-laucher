#define JAVAPATH       L"jre\\bin\\java.exe"
#define ARGS           L"--class-path lib/*;* com.example.MyApplication"

#include <windows.h>
#include <stdio.h>
#include <shlwapi.h>
#define BUFSIZE 512
wchar_t buf[BUFSIZE];

void cdToApplication() {
	wchar_t path[MAX_PATH];
	int len = GetModuleFileNameW(NULL, path, MAX_PATH);
	if (len > 0 && len < MAX_PATH) {
		path[len-1] = 0;
		if (PathRemoveFileSpecW(path) != 0) {
			SetCurrentDirectoryW(path);
		}
	}
}

int main() {
	cdToApplication();
	STARTUPINFOW si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	LPCWSTR cmd = JAVAPATH;
	WCHAR args[] = JAVAPATH L" " ARGS;
	if (CreateProcessW(cmd, args, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi) == 0) {
		swprintf_s( buf, BUFSIZE, L"Could not run %s\nerror code: %lu\ncommand line: %s", cmd, GetLastError(), args );
		MessageBoxExW(NULL, buf, L"Message", MB_OK | MB_ICONERROR, 0);
		return 0;
	}

	WaitForSingleObject(pi.hProcess, INFINITE);
	DWORD exitCode = 0;
	if (GetExitCodeProcess(pi.hProcess, &exitCode) != 0) {
		if (exitCode != 0) {
			swprintf_s( buf, BUFSIZE, L"%s\nfailed with exit code %lu\ncommand line: %s", cmd, exitCode, args );
			MessageBoxExW(NULL, buf, L"Message", MB_OK | MB_ICONERROR, 0);
		}
	}
	else {
		swprintf_s( buf, BUFSIZE, L"%s\nterminated with unknown exit code\ncommand line: %s", cmd, args );
		MessageBoxExW(NULL, buf, L"Message", MB_OK | MB_ICONERROR, 0);
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return 0;
}
