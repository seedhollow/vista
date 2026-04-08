#include "pch.h"
#include "c_uncensored_feature.h"
#include "cheat/sdk/entity_render_helper.h"

namespace Features {

	using UT = UnityResolve::UnityType;

	void hk_EntityRenderHelper_SetManualDitherAlphaValue(void* _this, float alpha) {

		if (player_func->peeking) {
			alpha = 1.f;
		}

		CALL_ORIGIN(hk_EntityRenderHelper_SetManualDitherAlphaValue, _this, alpha);
	}

	

	void CUncensoredFeature::Initialize() {
		HookManager::install(SDK::EntityRenderHelper::SetManualDitherAlphaValue->function, hk_EntityRenderHelper_SetManualDitherAlphaValue);
	}
}