#pragma once

namespace SDK {
	using UR = UnityResolve;

	class BattleRecorder {
	public:
		static UR::Class* GetClass();
		static void Initialize( );
		inline static UR::Method* SetAntiCheatRecordWindow = nullptr;
		inline static UR::Method* SaveAnticheatReport = nullptr;
		inline static UR::Method* SetInDetailRecording = nullptr;
		inline static UR::Method* RecordDamage = nullptr;
		inline static UR::Method* RecordHeal = nullptr;
		inline static UR::Method* RecordGainUltimateSp = nullptr;
		inline static UR::Method* RecordShield = nullptr;
		inline static UR::Method* RecordGainAtb = nullptr;
		inline static UR::Method* RecordConsumeAtb = nullptr;
		inline static UR::Method* RecordRecoverAtb = nullptr;
		inline static UR::Method* RecordConsumeDashCount = nullptr;



		inline static UR::Method* _RecordMoveDetail = nullptr;
		inline static UR::Method* RecordMoveDetails = nullptr;
		inline static UR::Method* Tick = nullptr;
		inline static UR::Method* _ResetDetailReport = nullptr;
		inline static UR::Method* _DoEndRecord = nullptr;
		inline static UR::Method* ResetAll = nullptr;
		inline static UR::Method* BeginRecord = nullptr;
		inline static UR::Method* EndRecord = nullptr;


	private:
		inline static UR::Class* pClass = nullptr;
		inline static bool initialized = false;
	};
} // namespace SDK
