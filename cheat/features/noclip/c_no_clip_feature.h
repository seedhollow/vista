#pragma once
#include "../feature_base.h"

namespace Features {
	
	class CNoClipFeature : public IFeature {
	public:
		void Initialize() override;
		void Update() override;
	};
	inline CNoClipFeature* g_CNoClipFeature = nullptr;
}