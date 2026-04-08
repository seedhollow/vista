#include "pch.h"
#include "c_nocooldown_feature.h"
#include "cheat/sdk/object_type_enum.h"

namespace Features {

	static float hk_Skill_get_cooldown(void* __this) {
		if (player_func->no_cooldown) {
			return 0.f;
		}
		return CALL_ORIGIN(hk_Skill_get_cooldown, __this);
	}

	static bool hk_Skill_CheckCd(void* __this) {
		if (__this) {
			if (player_func->no_skill_cost) {
				return true;
			}
		}
		return CALL_ORIGIN(hk_Skill_CheckCd, __this);
	}

	static void hk_Skill__ApplyCost(void* __this) {
		if (__this) {
			if (player_func->no_skill_cost) {
				return;
			}
		}
		CALL_ORIGIN(hk_Skill__ApplyCost, __this);
	}

	static bool hk_Skill_CheckTag(void* __this, void* tag) {
		if (__this) {
			if (player_func->no_skill_cost) {
				return true;
			}
		}
		return CALL_ORIGIN(hk_Skill_CheckTag, __this, tag);
	}
	
	void CNocooldownFeature::Initialize() {
		LOG("[+] CNocooldownFeature initialized\n");
		HookManager::install(SDK::Skill::get_cooldown->function, hk_Skill_get_cooldown);
		HookManager::install(SDK::Skill::CheckCd->function, hk_Skill_CheckCd);
		HookManager::install(SDK::Skill::_ApplyCost->function, hk_Skill__ApplyCost);
		HookManager::install(SDK::Skill::CheckTag->function, hk_Skill_CheckTag);
	}
}