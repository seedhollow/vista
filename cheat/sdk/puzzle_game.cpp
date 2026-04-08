#include "pch.h"
#include "puzzle_game.h"

namespace SDK {
	UR::Class* PuzzleGame::GetClass() {
		if (!pClass) {
			pClass = UR::Get("Gameplay.Beyond.dll")->Get("PuzzleGame");
		}
		return pClass;
	}

	void PuzzleGame::Initialize() {
		if (initialized)
			return;
		if (!GetClass()) {
			LOG("[-] PuzzleGame class not found!\n");
			return;
		}
		IsPuzzleComplete				= GetClass()->Get<UR::Method>("IsPuzzleComplete");
		DebugCompleteCurrentChessboard	= GetClass()->Get<UR::Method>("DebugCompleteCurrentChessboard");
		initialized = true;
		LOG("[+] PuzzleGame initialized.\n");
		LOG("[*] PuzzleGame::IsPuzzleComplete: %p\n", IsPuzzleComplete->function);
		LOG("[*] PuzzleGame::DebugCompleteCurrentChessboard: %p\n", DebugCompleteCurrentChessboard->function);
	}
}