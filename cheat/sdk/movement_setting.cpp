#include "pch.h"
#include "movement_setting.h"

namespace SDK {
	UR::Class* MovementSetting::GetClass() {
		if (!pClass) {
			pClass = UR::Get("Gameplay.Beyond.dll")->Get("MovementSetting");
		}
		return pClass;
	}

	void MovementSetting::Initialize() {
		if (initialized)
			return;
		if (!GetClass()) {
			LOG("[-] MovementSetting class not found!\n");
			return;
		}

		get_maxJumpHeight		= GetClass()->Get<UR::Method>("get_maxJumpHeight");
		Modify_maxJumpHeight	= GetClass()->Get<UR::Method>("Modify_maxJumpHeight", { "System.Single" });

		get_gravity				= GetClass()->Get<UR::Method>("get_gravity");
		Modify_gravity			= GetClass()->Get<UR::Method>("Modify_gravity", { "System.Single" });

		LOG("[+] MovementSetting initialized.\n");
		LOG("[*] MovementSetting::get_maxJumpHeight: %p\n", get_maxJumpHeight->function);
		LOG("[*] MovementSetting::Modify_maxJumpHeight: %p\n", Modify_maxJumpHeight->function);
		LOG("[*] MovementSetting::get_gravity: %p\n", get_gravity->function);
		LOG("[*] MovementSetting::Modify_gravity: %p\n", Modify_gravity->function);
		initialized = true;
	}

}