#pragma once

namespace SDK {
	using UR = UnityResolve;

	class Chessboard {
	public:
		static UR::Class* GetClass();
		static void Initialize();
		inline static UR::Method* CheckComplete = nullptr;
	private:
		inline static UR::Class* pClass = nullptr;
		inline static bool initialized = false;
	};
} // namespace SDK
