#pragma once

#include "../feature_base.h"

namespace Features {

	class CPlayerGravityFeature : public IFeature {
	public:
		void Initialize() override;
		void Update() override;
	};
	inline CPlayerGravityFeature* g_CPlayerGravityFeature = nullptr;
}