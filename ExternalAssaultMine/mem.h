#pragma once
#include <Windows.h>

namespace mem {
	void PatchEx(HANDLE hProcess, uintptr_t destination, uintptr_t newValue);
}
