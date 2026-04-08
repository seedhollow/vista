#pragma once
#include "../feature_base.h"

namespace Features {
	class CCameraFovFeature : public IFeature {
	public:
		void Initialize() override;
	};
	inline CCameraFovFeature* g_CCameraFovFeature = nullptr;
}