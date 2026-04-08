#include "pch.h"
#include "battle_recorder.h"

namespace SDK {
	UR::Class* BattleRecorder::GetClass() {
		if (!pClass) {
			pClass = UR::Get("Gameplay.Beyond.dll")->Get("BattleRecorder");
		}
		return pClass;
	}

	void BattleRecorder::Initialize( ) {
		if (initialized)
			return;

		if (!GetClass()) {
			LOG("[-] BattleRecorder class not found!\n");
			return;
		}

		SetAntiCheatRecordWindow = GetClass()->Get<UR::Method>("SetAntiCheatRecordWindow", {"System.Single"});
		SaveAnticheatReport		 = GetClass()->Get<UR::Method>("SaveAnticheatReport", {"System.Single"});
		SetInDetailRecording	 = GetClass()->Get<UR::Method>("SetInDetailRecording", { "Beyond.Gameplay.Core.BattleManager.BattleRecorder.DetailRecordLevel" });

		RecordDamage			 = GetClass()->Get<UR::Method>("RecordDamage", { "Beyond.Gameplay.Core.AbilitySystem.Modifier&" });
		RecordHeal				 = GetClass()->Get<UR::Method>("RecordHeal", { "Beyond.Gameplay.Core.AbilitySystem.Modifier&" });
		RecordGainUltimateSp	 = GetClass()->Get<UR::Method>("RecordGainUltimateSp", { "Beyond.Gameplay.Core.AbilitySystem.Modifier&" });
		RecordShield			 = GetClass()->Get<UR::Method>("RecordShield", { "Beyond.Gameplay.Core.AbilitySystem", "Beyond.Gameplay.Core.AbilitySystem", "System.Double" });
		RecordGainAtb			 = GetClass()->Get<UR::Method>("RecordGainAtb", { "Beyond.Gameplay.Core.AbilitySystem", "System.Double", "System.String", "Beyond.Gameplay.Core.Skill.SkillCastInfo" });
		RecordConsumeAtb		 = GetClass()->Get<UR::Method>("RecordConsumeAtb", { "Beyond.Gameplay.Core.AbilitySystem", "System.Double" });
		RecordRecoverAtb		 = GetClass()->Get<UR::Method>("RecordRecoverAtb", { "System.Single", "System.Collections.Generic.List`1<System.String>" });
		RecordConsumeDashCount	 = GetClass()->Get<UR::Method>("RecordConsumeDashCount", { "Beyond.Gameplay.Core.AbilitySystem", "System.Double" });

		_RecordMoveDetail		 = GetClass()->Get<UR::Method>("_RecordMoveDetail", { "Beyond.Gameplay.Core.Entity" });
		RecordMoveDetails		 = GetClass()->Get<UR::Method>("RecordMoveDetails", { "Beyond.Gameplay.Core.Entity" });
		Tick					 = GetClass()->Get <UR::Method>("Tick", { "System.Single" });
		_ResetDetailReport		 = GetClass()->Get<UR::Method>("_ResetDetailReport");
		_DoEndRecord			 = GetClass()->Get<UR::Method>("_DoEndRecord");
		ResetAll				 = GetClass()->Get<UR::Method>("ResetAll");
		BeginRecord				 = GetClass()->Get<UR::Method>("BeginRecord");
		EndRecord			     = GetClass()->Get<UR::Method>("EndRecord");




		initialized = true;

		LOG("[+] BattleRecorder initialized.\n");
		LOG("[*] BattleRecorder::SetAntiCheatRecordWindow: %p\n", SetAntiCheatRecordWindow->function);
		LOG("[*] BattleRecorder::SaveAnticheatReport: %p\n", SaveAnticheatReport->function);
		LOG("[*] BattleRecorder::SetInDetailRecording: %p\n", SetInDetailRecording->function);
		LOG("[*] BattleRecorder::RecordDamage: %p\n", RecordDamage->function);
		LOG("[*] BattleRecorder::RecordHeal: %p\n", RecordHeal->function);
		LOG("[*] BattleRecorder::RecordGainUltimateSp: %p\n", RecordGainUltimateSp->function);
		LOG("[*] BattleRecorder::RecordShield: %p\n", RecordShield->function);
		LOG("[*] BattleRecorder::RecordGainAtb: %p\n", RecordGainAtb->function);
		LOG("[*] BattleRecorder::RecordConsumeAtb: %p\n", RecordConsumeAtb->function);
		LOG("[*] BattleRecorder::RecordRecoverAtb: %p\n", RecordRecoverAtb->function);
		LOG("[*] BattleRecorder::RecordConsumeDashCount: %p\n", RecordConsumeDashCount->function);
		LOG("[*] BattleRecorder::_RecordMoveDetail: %p\n", _RecordMoveDetail->function);
		LOG("[*] BattleRecorder::RecordMoveDetails: %p\n", RecordMoveDetails->function);
		LOG("[*] BattleRecorder::Tick: %p\n", Tick->function);
		LOG("[*] BattleRecorder::_ResetDetailReport: %p\n", _ResetDetailReport->function);
		LOG("[*] BattleRecorder::_DoEndRecord: %p\n", _DoEndRecord->function);
		LOG("[*] BattleRecorder::ResetAll: %p\n", ResetAll->function);
		LOG("[*] BattleRecorder::BeginRecord: %p\n", BeginRecord->function);
		LOG("[*] BattleRecorder::EndRecord: %p\n", EndRecord->function);

	}
} // namespace SDK
