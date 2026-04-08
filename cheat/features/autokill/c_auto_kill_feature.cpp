#include "pch.h"
#include "c_auto_kill_feature.h"
#include "cheat/sdk/battle_manager.h"
#include "cheat/sdk/object_type_enum.h"

namespace Features {

	using UT = UnityResolve::UnityType;

	void hk_BattleManager_Tick(void* __this, float deltaTime) {
		if (__this) {
			// get all enemies and kill them
			if (player_func->auto_kill_enemies) {
				UT::List<void*>* enemies = SDK::BattleManager::get_enemies->Invoke<UT::List<void*>*>(__this);
				for (int i = 0; i < enemies->size; i++) {
					void* enemy = enemies->operator[](i);

					auto is_valid		= SDK::Entity::IsValid->Invoke<bool>(enemy);
					auto is_alive		= SDK::Entity::get_alive->Invoke<bool>(enemy);
					auto is_hide		= SDK::Entity::get_isHide->Invoke<bool>(enemy);
					auto is_mark_die	= SDK::Entity::get_markDie->Invoke<bool>(enemy);

					auto curr_ability	= SDK::Entity::get_abilityCom->Invoke<void*>(enemy);

					if (curr_ability && is_valid && is_alive && !is_hide && !is_mark_die) {
						SDK::AbilitySystem::SetHpDirectly->Invoke<void>(curr_ability, 0.0, true);
					}
				}
			}
		}
		CALL_ORIGIN(hk_BattleManager_Tick, __this, deltaTime);
	}

	void CAutoKillFeature::Initialize() {
		LOG("[+] CAutoKillFeature initialized\n");
		HookManager::install(SDK::BattleManager::Tick->function, hk_BattleManager_Tick);
	}
}