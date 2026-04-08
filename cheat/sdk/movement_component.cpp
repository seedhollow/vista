#include "pch.h"
#include "movement_component.h"

namespace SDK {
	UR::Class* MovementComponent::GetClass() {
		if (!pClass) {
			pClass = UR::Get("Gameplay.Beyond.dll")->Get("MovementComponent");
		}
		return pClass;
	}

	void MovementComponent::Initialize() {
		if (initialized)
			return;
		if (!GetClass()) {
			LOG("[-] MovementComponent class not found!\n");
			return;
		}

		get_config		= GetClass()->Get<UR::Method>("get_config");
		get_setting		= GetClass()->Get<UR::Method>("get_setting");
		SetSpeedMultiplierFromEnvironment = GetClass()->Get<UR::Method>("SetSpeedMultiplierFromEnvironment", { "System.Single" });	
		SetSpeed		= GetClass()->Get<UR::Method>("SetSpeed", { "System.Single" });
		Tick			= GetClass()->Get<UR::Method>("Tick", { "System.Single" });
		PreTick			= GetClass()->Get<UR::Method>("PreTick", { "System.Single" });
		set_moveMode	= GetClass()->Get<UR::Method>("set_moveMode", { "Beyond.Gameplay.Core.MovementComponent.MoveMode" });
		TeleportTo			= GetClass()->Get<UR::Method>("TeleportTo", { "UnityEngine.Vector3", "System.Boolean" });
		IsPositionWalkable	= GetClass()->Get<UR::Method>("IsPositionWalkable", { "UnityEngine.Vector3" });

		LOG("[+] MovementComponent initialized.\n");
		LOG("[*] MovementComponent::get_config: %p\n", get_config->function);
		LOG("[*] MovementComponent::get_setting: %p\n", get_setting->function);
		LOG("[*] MovementComponent::SetSpeedMultiplierFromEnvironment: %p\n", SetSpeedMultiplierFromEnvironment->function);
		LOG("[*] MovementComponent::SetSpeed: %p\n", SetSpeed->function);
		LOG("[*] MovementComponent::Tick: %p\n", Tick->function);
		LOG("[*] MovementComponent::PreTick: %p\n", PreTick->function);
		LOG("[*] MovementComponent::set_moveMode: %p\n", set_moveMode->function);
		LOG("[*] MovementComponent::TeleportTo: %p\n", TeleportTo->function);
		LOG("[*] MovementComponent::IsPositionWalkable: %p\n", IsPositionWalkable->function);

		initialized = true;
	}

}