#include "pch.h"
#include "c_chessboard_feature.h"

namespace Features {


	bool hk_Chessboard_CheckComplete(void* __this) {
		if (__this) {
			if (player_func->auto_solve_puzzles) {
				return true;
			}
		}
		return CALL_ORIGIN(hk_Chessboard_CheckComplete, __this);
	}

	bool hk_PuzzleGame_IsPuzzleComplete(void* __this) {
		if (__this) {
			if (player_func->auto_solve_puzzles) {
				SDK::PuzzleGame::DebugCompleteCurrentChessboard->Invoke<void>(__this);
				return true;
			}
		}
		return CALL_ORIGIN(hk_PuzzleGame_IsPuzzleComplete, __this);
	}


	void CChessboardFeature::Initialize() {
		LOG("[+] CChessboardFeature initialized\n");
		HookManager::install(SDK::Chessboard::CheckComplete->function, hk_Chessboard_CheckComplete);
		HookManager::install(SDK::PuzzleGame::IsPuzzleComplete->function, hk_PuzzleGame_IsPuzzleComplete);
	}
}