#include "pch.h"
#include "modifier.h"

namespace SDK {
	UR::Class* Modifier::GetClass() {
		if (!pClass) {
			pClass = UR::Get("Gameplay.Beyond.dll")->Get("Modifier");
		}
		return pClass;
	}

	void Modifier::Initialize() {
		if (initialized)
			return;
		if (!GetClass()) {
			LOG("[-] Modifier class not found!\n");
			return;
		}

		NewDamage	= GetClass()->Get<UR::Method>("NewDamage", { "Beyond.Gameplay.Core.AbilitySystem", "Beyond.Gameplay.Core.AbilitySystem", "System.Double", "System.Boolean", "System.Boolean", "Beyond.Gameplay.Core.DamagePackData&", "Beyond.Gameplay.Core.AbilitySystem.Modifier.DamageVisualData", "Beyond.Gameplay.Core.EnemyPartTransferInfo", "System.Int32", "System.Int32" });
		Apply		= GetClass()->Get<UR::Method>("Apply");
		LOG("[+] Modifier initialized.\n");
		LOG("[*] Modifier::NewDamage: %p\n", NewDamage->function);
		LOG("[*] Modifier::Apply: %p\n", Apply->function);

		initialized = true;

	}
}