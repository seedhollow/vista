#include "pch.h"
#include "chessboard.h"

namespace SDK {
	UR::Class* Chessboard::GetClass() {
		if (!pClass) {
			pClass = UR::Get("Gameplay.Beyond.dll")->Get("Chessboard");
		}
		return pClass;
	}

	void Chessboard::Initialize() {
		if (initialized)
			return;
		if (!GetClass()) {
			LOG("[-] Chessboard class not found!\n");
			return;
		}
		CheckComplete = GetClass()->Get < UR::Method>("CheckComplete");
		LOG("[+] Chessboard initialized.\n");
		LOG("[*] Chessboard::CheckComplete: %p\n", CheckComplete->function);
		initialized = true;
	}
}