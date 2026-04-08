#pragma once
namespace SDK {
	using UR = UnityResolve;
	class PuzzleGame {
	public:
		static UR::Class* GetClass();
		static void Initialize();
		// Function pointers
		inline static UR::Method* IsPuzzleComplete = nullptr;
		inline static UR::Method* DebugCompleteCurrentChessboard = nullptr;

	private:
		inline static bool initialized = false;
		inline static UR::Class* pClass = nullptr;
	};
} // namespace SDK