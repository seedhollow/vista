#include "pch.h"
#include "battle_manager.h"

namespace SDK {
	UR::Class* BattleManager::GetClass() {
		if (!pClass) {
			pClass = UR::Get("Gameplay.Beyond.dll")->Get("BattleManager");
		}
		return pClass;
	}

	void BattleManager::Initialize( ) {
		if (initialized)
			return;

		if (!GetClass()) {
			LOG("[-] BattleManager class not found!\n");
			return;
        }

		_Handle_AntiCheatCreditWarning  = GetClass()->Get<UR::Method>("_Handle_AntiCheatCreditWarning", {"SC_ANTI_CHEAT_CREDIT_WARNING"});
        _Handle_AntiCheatCreditZero		= GetClass()->Get<UR::Method>("_Handle_AntiCheatCreditZero", {"SC_ANTI_CHEAT_CREDIT_ZERO"});
		get_enemies						= GetClass()->Get<UR::Method>("get_enemies");
		_OnSquadInFight					= GetClass()->Get<UR::Method>("_OnSquadInFight", { "System.Boolean", "System.Boolean" });
		_TickSquadInFight				= GetClass()->Get<UR::Method>("_TickSquadInFight", { "System.Single" });
		Tick							= GetClass()->Get<UR::Method>("Tick", { "System.Single" });
		RegisterEntity					= GetClass()->Get<UR::Method>("RegisterEntity", { "Beyond.Gameplay.Core.Entity" });
		UnRegisterEntity				= GetClass()->Get<UR::Method>("UnRegisterEntity", { "Beyond.Gameplay.Core.Entity" });
		get_battleRecorder				= GetClass()->Get<UR::Method>("get_battleRecorder");

		LOG("[+] BattleManager initialized.\n");
		LOG("[*] BattleManager::_Handle_AntiCheatCreditWarning: %p\n", _Handle_AntiCheatCreditWarning->function);
		LOG("[*] BattleManager::_Handle_AntiCheatCreditZero: %p\n", _Handle_AntiCheatCreditZero->function);
		LOG("[*] BattleManager::get_enemies: %p\n", get_enemies->function);
		LOG("[*] BattleManager::_OnSquadInFight: %p\n", _OnSquadInFight->function);
		LOG("[*] BattleManager::_TickSquadInFight: %p\n", _TickSquadInFight->function);
		LOG("[*] BattleManager::Tick: %p\n", Tick->function);
		LOG("[*] BattleManager::RegisterEntity: %p\n", RegisterEntity->function);
		LOG("[*] BattleManager::UnregisterEntity: %p\n", UnRegisterEntity->function);
		LOG("[*] BattleManager::get_battleRecorder: %p\n", get_battleRecorder->function);

		initialized = true;
	}
} // namespace SDK
