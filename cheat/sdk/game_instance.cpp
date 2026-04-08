#include "pch.h"
#include "game_instance.h"

namespace SDK {
	UR::Class* GameInstance::GetClass() {
		if (!pClass) {
			pClass = UR::Get("Gameplay.Beyond.dll")->Get("GameInstance");
		}
		return pClass;
	}

	void GameInstance::Initialize() {
		if (initialized) return;

		if (!GetClass()) {
			LOG("[-] GameInstance class not found!\n");
			return;
		}

		get_instance		= GetClass()->Get<UR::Method>("get_instance");



		f_m_world			= GetClass()->Get<UR::Field>("m_world");
		f_cameraManager		= GetClass()->Get<UR::Field>("cameraManager");
		f_m_player			= GetClass()->Get<UR::Field>("m_player");
		initialized = true;

		LOG("[+] GameInstance initialized.\n");
		LOG("[*] GameInstance::get_instance: %p\n", get_instance->function);
		LOG("[*] GameInstance::f_m_world: %p\n", f_m_world->address);
		LOG("[*] GameInstance::f_cameraManager: %p\n", f_cameraManager->address);
		LOG("[*] GameInstance::f_m_player: %p\n", f_m_player->address);

	}


} // namespace SDK