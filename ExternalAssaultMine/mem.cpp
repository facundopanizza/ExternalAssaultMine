#include "mem.h"

void mem::PatchEx(HANDLE hProcess, uintptr_t destination, uintptr_t newValue) {
	DWORD oldProtect;

	VirtualProtectEx(hProcess, (BYTE*)destination, sizeof(newValue), PAGE_EXECUTE_READWRITE, &oldProtect);
	WriteProcessMemory(hProcess, (BYTE*)destination, &newValue, sizeof(newValue), nullptr);
	VirtualProtectEx(hProcess, (BYTE*)destination, sizeof(newValue), oldProtect, &oldProtect);
}
