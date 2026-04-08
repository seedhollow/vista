#include "pch.h"
#include "c_infinite_ultimate_feature.h"

namespace Features
{
	void CInfiniteUltimateFeature::Initialize() {
	}

	void CInfiniteUltimateFeature::Update() {
		if (player_func->infinite_skill_ultimate) {
			auto player = SDK::GameUtil::get_mainCharacter->Invoke<void*>();
			if (player) {
				auto ability_system = SDK::Entity::get_abilityCom->Invoke<void*>(player);
				if (ability_system) {
					// idk how much the max ultimate is but 50000 should be enough for infinite ultimate
					SDK::AbilitySystem::set_ultimateSp->Invoke<void>(ability_system, 50000.f);
				}
			}
		}
	}
}