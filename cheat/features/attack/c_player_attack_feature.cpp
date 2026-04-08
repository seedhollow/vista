#include "pch.h"
#include "c_player_attack_feature.h"
#include "cheat/sdk/object_type_enum.h"
#include "cheat/tools/entity_manager.h"

namespace Features {
	static void* Hk_Modifier_NewDamage(
		void* ptr,
		void* source,
		void* target,
		double value,
		bool isCritical,
		bool isBlocked,
		void** damagePackData,
		void* damageVisualData,
		void* partTransferInfo,
		int serverActionIndex,
		int damageUnitIndex
	) {
		if (source && target) {
			auto source_type = SDK::AbilitySystem::get_objectType->Invoke<ObjectType_enum>(source);
			auto target_type = SDK::AbilitySystem::get_objectType->Invoke<ObjectType_enum>(target);

			if (target_type == ObjectType_enum::Enemy) {
				if (player_func->always_crit) {
					isCritical = true;
				}

				if (player_func->damage_multiplier) {
					value *= player_func->damage_multiplier_value;
				}

				if (player_func->hit_multiplier) {
					for (int i = 0; i < player_func->hit_multiplier_value -1; i++) { // -1 because the original hit is already counted
						auto result = CALL_ORIGIN(Hk_Modifier_NewDamage, ptr, source, target, value, isCritical, isBlocked, damagePackData, damageVisualData, partTransferInfo, serverActionIndex, damageUnitIndex);
						if (result) {
							SDK::Modifier::Apply->Invoke<void>(result);
						}
					}
				}
			}

			if (target_type == ObjectType_enum::Character) {
			}
		}
		return CALL_ORIGIN(Hk_Modifier_NewDamage, ptr, source, target, value, isCritical, isBlocked, damagePackData, damageVisualData, partTransferInfo, serverActionIndex, damageUnitIndex);
	}


	void CPlayerAttackFeature::Initialize() {
		LOG("[+] CPlayerAttackFeature initialized\n");
		HookManager::install(SDK::Modifier::NewDamage->function, Hk_Modifier_NewDamage);
	}

	void CPlayerAttackFeature::Update() {
		AttackSpeed();
	}

	void CPlayerAttackFeature::AttackSpeed() {

	}
}