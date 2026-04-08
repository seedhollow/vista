#include "pch.h"
#include "object_container.h"

namespace SDK {
	UR::Class* ObjectContainer::GetClass() {
		if (!pClass) {
			pClass = UR::Get("Gameplay.Beyond.dll")->Get("ObjectContainer");
		}
		return pClass;
	}

	void ObjectContainer::Initialize( ) {
		if (initialized)
			return;
		if (!GetClass()) {
			LOG("[-] ObjectContainer class not found!\n");
			return;
		}

		Tick						= GetClass()->Get<UR::Method>("Tick", { "System.Single" });
		get_allEntities				= GetClass()->Get<UR::Method>("get_allEntities");
		get_allCharacters			= GetClass()->Get<UR::Method>("get_allCharacters");
		get_allEnemies				= GetClass()->Get<UR::Method>("get_allEnemies");
		get_allInteractives			= GetClass()->Get<UR::Method>("get_allInteractives");
		get_allAbilityEntities		= GetClass()->Get<UR::Method>("get_allAbilityEntities");
		Register					= GetClass()->Get<UR::Method>("Register", { "Beyond.Gameplay.Core.Entity", "System.UInt64" });
		UnRegister					= GetClass()->Get<UR::Method>("UnRegister", { "Beyond.Gameplay.Core.Entity" });
		GetInteractiveByDistance	= GetClass()->Get<UR::Method>("GetInteractiveByDistance", { "UnityEngine.Vector3", "System.Single" });

		initialized = true;
		LOG("[+] ObjectContainer initialized.\n");
		LOG("[*] ObjectContainer::Tick: %p\n", Tick->function);
		LOG("[*] ObjectContainer::get_allEntities: %p\n", get_allEntities->function);
		LOG("[*] ObjectContainer::get_allCharacters: %p\n", get_allCharacters->function);
		LOG("[*] ObjectContainer::get_allEnemies: %p\n", get_allEnemies->function);
		LOG("[*] ObjectContainer::get_allInteractives: %p\n", get_allInteractives->function);
		LOG("[*] ObjectContainer::get_allAbilityEntities: %p\n", get_allAbilityEntities->function);
		LOG("[*] ObjectContainer::Register: %p\n", Register->function);
		LOG("[*] ObjectContainer::UnRegister: %p\n", UnRegister->function);
		LOG("[*] ObjectContainer::GetInteractiveByDistance: %p\n", GetInteractiveByDistance->function);

	}
} // namespace SDK
