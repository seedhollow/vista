#pragma once
#include "../feature_base.h"

namespace Features {
	
	class CWorldAmbienceFeature : public IFeature {
	public:
		void Initialize() override;
		void Update() override;
	};
	inline CWorldAmbienceFeature* g_CWorldAmbienceFeature = nullptr;
}