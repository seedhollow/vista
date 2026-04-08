#include "pch.h"
#include "c_player_gravity_feature.h"

namespace Features {
	void CPlayerGravityFeature::Initialize() {
		LOG("[+] CPlayerGravityFeature initialized\n");
	}
	void CPlayerGravityFeature::Update() {
		auto player_entity = SDK::GameUtil::get_mainCharacter->Invoke<void*>();
		if (!player_entity) return;

		auto movement_component = SDK::Entity::get_movementComponent->Invoke<void*>(player_entity);
		if (movement_component) {
			auto movement_setting = SDK::MovementComponent::get_setting->Invoke<void*>(movement_component);
			if (movement_setting) {
				auto current_gravity = SDK::MovementSetting::get_gravity->Invoke<float>(movement_setting);

				// 40.0 is the default gravity value
			}
		}
	}
}