#include "pch.h"
#include "c_game_tick_feature.h"
#include "cheat/features/feature_manager.h"
#include "cheat/sdk/object_container.h"

namespace Features {

	using UT = UnityResolve::UnityType;
	using UR = UnityResolve;

    void hk_ObjectContainer_Tick(void* __this, float deltaTime) {
        if (__this) {
            Features::FeatureManager::Update();
        }

		CALL_ORIGIN(hk_ObjectContainer_Tick, __this, deltaTime);
	}

    void CGameTickFeature::Initialize() {
		LOG("[+] CBaseTickManagerFeature initialized\n");
		HookManager::install(SDK::ObjectContainer::Tick->function, hk_ObjectContainer_Tick);
    }
}