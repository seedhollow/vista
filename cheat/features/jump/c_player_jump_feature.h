#pragma once
#include "../feature_base.h"

namespace Features {
	class CPlayerJumpFeature : public IFeature {
	public:
		void Initialize() override;
		void Update() override;

	private:
		float original_jump_height = 2.1f;
	};

	inline CPlayerJumpFeature* g_CPlayerJumpFeature = nullptr;
}