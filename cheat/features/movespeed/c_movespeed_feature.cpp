#include "pch.h"
#include "c_movespeed_feature.h"

namespace Features {
	
	void CMovespeedFeature::Update() {
		auto player_entity = SDK::GameUtil::get_mainCharacter->Invoke<void*>();
		
		if (player_entity) {
			auto root_com = SDK::Entity::get_rootCom->Invoke<void*>(player_entity);
			auto movement_component = SDK::Entity::get_movementComponent->Invoke<void*>(player_entity);
			if (movement_component) {
				SDK::MovementComponent::SetSpeedMultiplierFromEnvironment->Invoke<void>(movement_component, static_cast<float>(player_func->movespeed_multiplier_value));
			}
		}
	}

	void CMovespeedFeature::Initialize() {

	}
}