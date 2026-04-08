#include "pch.h"
#include "c_player_jump_feature.h"

namespace Features {
	void CPlayerJumpFeature::Initialize() {
		LOG("[+] CPlayerJumpFeature initialized\n");
	}

	void CPlayerJumpFeature::Update() {
		if (player_func->jump_multiplier) {
			auto player_entity = SDK::GameUtil::get_mainCharacter->Invoke<void*>();
			if (!player_entity) return;

			auto movement_component = SDK::Entity::get_movementComponent->Invoke<void*>(player_entity);
			if (movement_component) {
				auto movement_setting = SDK::MovementComponent::get_setting->Invoke<void*>(movement_component);
				if (movement_setting) {
					auto new_jump_height = original_jump_height * static_cast<float>(player_func->jump_multiplier_value);
					SDK::MovementSetting::Modify_maxJumpHeight->Invoke<void>(movement_setting, new_jump_height);
				}
			}
		}
	}
}