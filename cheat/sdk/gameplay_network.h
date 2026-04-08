#pragma once
namespace SDK {
	using UR = UnityResolve;

	class GameplayNetwork {
	public:
		static UR::Class* GetClass();
		static void Initialize( );
		// Function pointers
		inline static UR::Method* _HandleAntiCheatTicket = nullptr;
		inline static UR::Method* _HandleAntiCheatSyncData = nullptr;
	private:
		inline static bool initialized = false;
		inline static UR::Class* pClass = nullptr;
	};
} // namespace SDK
