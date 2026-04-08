#include "pch.h"
#include "gameplay_network.h"


namespace SDK {
	UR::Class* GameplayNetwork::GetClass() {
		if (!pClass) {
			pClass = UR::Get("Gameplay.Beyond.dll")->Get("GameplayNetwork");
		}
		return pClass;
	}

	void GameplayNetwork::Initialize( ) {
		if (initialized)
			return;


		if (!GetClass()) {
			LOG("[-] GameplayNetwork class not found!\n");
			return;
        }

		_HandleAntiCheatTicket	 = GetClass()->Get<UR::Method>("_HandleAntiCheatTicket", {"SC_ANTI_CHEAT_TICKET"});
		_HandleAntiCheatSyncData = GetClass()->Get<UR::Method>("_HandleAntiCheatSyncData", {"SC_ANTI_CHEAT_SYNC_DATA"});
		initialized = true;
		LOG("[+] GameplayNetwork initialized.\n");
		LOG("[*] GameplayNetwork::_HandleAntiCheatTicket: %p\n", _HandleAntiCheatTicket->function);
		LOG("[*] GameplayNetwork::_HandleAntiCheatSyncData: %p\n", _HandleAntiCheatSyncData->function);
	}
} // namespace SDK
