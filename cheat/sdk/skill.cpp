#include "pch.h"
#include "skill.h"

namespace SDK {
	UR::Class* Skill::GetClass() {
		if (!pClass) {
			pClass = UR::Get("Gameplay.Beyond.dll")->Get("Skill");
		}
		return pClass;
	}
	void Skill::Initialize() {
		if (initialized)
			return;
		if (!GetClass()) {
			LOG("[-] Skill class not found!\n");
			return;
		}

		get_cooldown	= GetClass()->Get<UR::Method>("get_cooldown");
		get_owner		= GetClass()->Get<UR::Method>("get_owner");
		_ApplyCost		= GetClass()->Get<UR::Method>("_ApplyCost");
		CheckCd			= GetClass()->Get<UR::Method>("CheckCd");
		CheckTag		= GetClass()->Get<UR::Method>("CheckTag");


		initialized = true;
		LOG("[+] Skill initialized.\n");
		LOG("[*] Skill::get_cooldown: %p\n", get_cooldown->function);
		LOG("[*] Skill::get_owner: %p\n", get_owner->function);
		LOG("[*] Skill::_ApplyCost: %p\n", _ApplyCost->function);
		LOG("[*] Skill::CheckCd: %p\n", CheckCd->function);
		LOG("[*] Skill::CheckTag: %p\n", CheckTag->function);

	}
} // namespace SDK