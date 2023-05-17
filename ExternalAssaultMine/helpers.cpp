#include "helpers.h"

uintptr_t helpers::findDMA(HANDLE hProcess, uintptr_t ptr, std::vector<unsigned int> offsets) {
	uintptr_t address = ptr;

	for (int i = 0; i < offsets.size(); i++) {
		address += offsets[i];
		ReadProcessMemory(hProcess, (BYTE*)address, &address, sizeof(address), nullptr);
	}

	return address;
}

DWORD helpers::getProcessId(const wchar_t* processName) {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 processInfo;
	DWORD processId = 0;

	processInfo.dwSize = sizeof(processInfo);

	if (Process32First(hSnapshot, &processInfo));

	do {
		if (!_wcsicmp(processInfo.szExeFile, processName)) {
			processId = processInfo.th32ProcessID;
			break;
		}
	} while (Process32Next(hSnapshot, &processInfo));

	CloseHandle(hSnapshot);

	if (processId == 0) {
		return 0;
	}

	return processId;
}	

DWORD helpers::getModuleAddress(const wchar_t* processName, DWORD processId) {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
	MODULEENTRY32 moduleInfo;

	moduleInfo.dwSize = sizeof(moduleInfo);
	uintptr_t modBaseAddr = 0;

	if (!Module32First(hSnapshot, &moduleInfo)) {
		CloseHandle(hSnapshot);
		return 0;
	}

	do {

		if (!_wcsicmp(moduleInfo.szModule, processName)) {
			modBaseAddr = (uintptr_t)moduleInfo.modBaseAddr;
			break;
		}
	} while (Module32Next(hSnapshot, &moduleInfo));

	CloseHandle(hSnapshot);

	if (modBaseAddr == 0) {
		return 0;
	}


	return modBaseAddr;
}
