#include "pch.h"
#include "game_world.h"

namespace SDK {
	UR::Class* GameWorld::GetClass() {
		if (!pClass) {
			pClass = UR::Get("Gameplay.Beyond.dll")->Get("GameWorld");
		}
		return pClass;
	}

	void GameWorld::Initialize() {
		if (initialized) return;
		if (!GetClass()) {
			LOG("[-] GameWorld class not found!\n");
			return;
		}

		f_battle = GetClass()->Get<UR::Field>("battle");

		initialized = true;
		LOG("[+] GameWorld initialized.\n");
		LOG("[*] GameWorld::f_battle: %p\n", f_battle->address);
	}

}// namespace SDK