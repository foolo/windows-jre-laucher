#define CLASSNAME      L"my.package.MyApplication"
#define JAVAPATH       L"jre\\bin\\java.exe"
#define ARGS           L"--class-path \"lib/*\"" CLASSNAME

#include <windows.h>
#include <sstream>
#include <shlwapi.h>

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
	WCHAR args[] = JAVAPATH ARGS;
	if (CreateProcessW(cmd, args, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi) == 0) {
		std::wstringstream ss;
		ss << "Could not run " << cmd << std::endl;
		ss << "error code: " << GetLastError() << std::endl;
		ss << "command line: " << args << std::endl;
		MessageBoxExW(NULL, ss.str().c_str(), L"Message", MB_OK | MB_ICONERROR, 0);
		return 0;
	}

	WaitForSingleObject(pi.hProcess, INFINITE);
	DWORD exitCode = 0;
	if (GetExitCodeProcess(pi.hProcess, &exitCode) != 0) {
		if (exitCode != 0) {
			std::wstringstream ss;
			ss << cmd << std::endl;
			ss << "failed with exit code " << exitCode << std::endl;
			ss << "command line: " << args << std::endl;
			MessageBoxExW(NULL, ss.str().c_str(), L"Message", MB_OK | MB_ICONERROR, 0);
		}
	}
	else {
		std::wstringstream ss;
		ss << cmd << std::endl;
		ss << "terminated with unknown exit code" << std::endl;
		ss << "command line: " << args << std::endl;
		MessageBoxExW(NULL, ss.str().c_str(), L"Message", MB_OK | MB_ICONERROR, 0);
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return 0;
}
