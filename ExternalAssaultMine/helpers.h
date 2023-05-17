#pragma once
#include <Windows.h>
#include <vector>
#include <TlHelp32.h>

namespace helpers {
	uintptr_t findDMA(HANDLE hProcess, uintptr_t ptr, std::vector<unsigned int> offsets);

	DWORD getProcessId(const wchar_t* processName);

	DWORD getModuleAddress(const wchar_t* processName, DWORD processId);
}
