#include "pch.h"
#include "c_infinite_stamina_feature.h"

namespace Features {


	static void hk_PlayerController_DecreaseDashCount(void* __this) {
		if (player_func->infinite_stamina) {
			return;
		}
		CALL_ORIGIN(hk_PlayerController_DecreaseDashCount, __this);
	}

	void CInfiniteStaminaFeature::Initialize() {
		LOG("[+] CInfiniteStaminaFeature initialized\n");

		HookManager::install(SDK::PlayerController::DecreaseDashCount->function, hk_PlayerController_DecreaseDashCount);
	}
}