#include "pch.h"
#include "c_entity_finder_feature.h"
#include "cheat/tools/entity_manager.h"

namespace Features {

	static void hk_ObjectContainer_Register(void* __this, void* entity, uint64_t entityId) {
		if (__this && entity) {
			auto ability_system = SDK::Entity::get_abilityCom->Invoke<void*>(entity);

			Entity newEntity;
			newEntity.ptr = entity;
			Manager::EntityManager::GetInstance().AddEntity(newEntity);
		}

		CALL_ORIGIN(hk_ObjectContainer_Register, __this, entity, entityId);
	}

	static void hk_ObjectContainer_UnRegister(void* __this, void* entity) {
		if (__this && entity) {
			Entity toRemove;
			toRemove.ptr = entity;
			Manager::EntityManager::GetInstance().RemoveEntity(toRemove);
		}

		CALL_ORIGIN(hk_ObjectContainer_UnRegister, __this, entity);
	}

	void CEntityFinderFeature::Update() {
		
	}


	void CEntityFinderFeature::Initialize() {
		LOG("[+] CEntityFinderFeature initialized\n");
		HookManager::install(SDK::ObjectContainer::Register->function, hk_ObjectContainer_Register);
		HookManager::install(SDK::ObjectContainer::UnRegister->function, hk_ObjectContainer_UnRegister);
	}
}