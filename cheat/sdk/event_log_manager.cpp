#include "pch.h"
#include "event_log_manager.h"


namespace SDK {
    UR::Class* EventLogManager::GetClass() {
        if (!pClass) {
            pClass = UR::Get("Common.Beyond.dll")->Get("EventLogManager","Beyond.SDK");
        }
		return pClass;
    }

    void EventLogManager::Initialize( ) {
        if (initialized)
            return;

        if (!GetClass()) {
            LOG("[-] EventLogManager class not found!\n");
            return;
        }

        GameEvent_BattleAnticheat = GetClass()->Get<UR::Method>("GameEvent_BattleAnticheat", {"System.String"});
        LOG("[+] EventLogManager initialized.\n");
		LOG("[*] EventLogManager::GameEvent_BattleAnticheat: %p\n", GameEvent_BattleAnticheat->function);
        initialized = true;
    }
}
