#include "pch.h"
#include "c_godmode_feature.h"

namespace Features {

	using UT = UnityResolve::UnityType;
	using UR = UnityResolve;

	void CGodModeFeature::Initialize() {
		LOG("[+] CGodModeFeature initialized\n");
	}

	void CGodModeFeature::Update() {
		if (player_func->godmode) {
			auto player = SDK::GameUtil::get_mainCharacter->Invoke<void*>();
			if (player) {
				auto ability_system = SDK::Entity::get_abilityCom->Invoke<void*>(player);

				if (ability_system) {
					float max_hp = SDK::AbilitySystem::get_maxHp->Invoke<float>(ability_system);
					SDK::AbilitySystem::SetHpDirectly->Invoke<void>(ability_system, max_hp, true);
				}
			}
		}
	}
}