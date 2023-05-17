#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include "mem.h"
#include "helpers.h"

void mainLoop(HANDLE hProcess, uintptr_t modBaseAddr) {
	const uintptr_t playerPtr = modBaseAddr + 0x109B74;
	uintptr_t playerAddr;

	ReadProcessMemory(hProcess, (BYTE*)playerPtr, &playerAddr, sizeof(playerPtr), nullptr);

	uintptr_t healthAddr = playerAddr + 0xF8;
	uintptr_t armorAddr = playerAddr + 0xFC;
	uintptr_t currentAmmo = helpers::findDMA(hProcess, playerAddr, { 0x374, 0x14 });

	int newAmmo = 10, newHealth = 1337, newArmor = 999;
	bool bAmmo = false, bHealth = false, bArmor = false;

	while (true) {
		if (GetAsyncKeyState(VK_INSERT) & 1) {
			break;
		}


		if (GetAsyncKeyState(VK_NUMPAD1) & 1) {
			bHealth = !bHealth;
	
			if (!bHealth)
				mem::PatchEx(hProcess, healthAddr, 100);
		}

		if (GetAsyncKeyState(VK_NUMPAD2) & 1) {
			bArmor = !bArmor;

			if (!bArmor)
				mem::PatchEx(hProcess, armorAddr, 0);
		}

		if (GetAsyncKeyState(VK_NUMPAD3) & 1) {
			bAmmo = !bAmmo;

			if (!bAmmo)
				mem::PatchEx(hProcess, currentAmmo, 10);
		}
		
		if (bHealth)
			mem::PatchEx(hProcess, healthAddr, newHealth);

		if (bAmmo)
			mem::PatchEx(hProcess, currentAmmo, newAmmo);

		if (bArmor)
			mem::PatchEx(hProcess, armorAddr, newArmor);

		Sleep(10);
	}
}

int main()
{
	const wchar_t* processName = L"ac_client.exe";

	DWORD processId = helpers::getProcessId(processName);
	uintptr_t modBaseAddr = helpers::getModuleAddress(processName, processId);

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, processId);

	mainLoop(hProcess, modBaseAddr);

	CloseHandle(hProcess);
	return 0;
}
