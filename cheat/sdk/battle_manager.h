#pragma once

namespace SDK {
	using UR = UnityResolve;

	class BattleManager {
	public:
		static UR::Class* GetClass();
		static void Initialize( );
		inline static UR::Method* _Handle_AntiCheatCreditWarning = nullptr;
        inline static UR::Method* _Handle_AntiCheatCreditZero = nullptr;
		inline static UR::Method* get_enemies = nullptr;
		inline static UR::Method* _OnSquadInFight = nullptr;
		inline static UR::Method* _TickSquadInFight = nullptr;
		inline static UR::Method* Tick = nullptr;
		inline static UR::Method* RegisterEntity = nullptr;
		inline static UR::Method* UnRegisterEntity = nullptr;
		inline static UR::Method* get_battleRecorder = nullptr;
	private:
		inline static UR::Class* pClass = nullptr;
		inline static bool initialized = false;
	};
} // namespace SDK
