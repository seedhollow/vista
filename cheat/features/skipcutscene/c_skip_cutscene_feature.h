#pragma once
#include "../feature_base.h"

namespace Features {

	class CSkipCutsceneFeature : public IFeature {
	public:
		void Initialize() override;
		void Update() override;
	};
	inline CSkipCutsceneFeature* g_CSkipCutsceneFeature = nullptr;
}