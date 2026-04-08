#pragma once
#include "../feature_base.h"

namespace Features {
    class CMovespeedFeature : public IFeature {
    public:
        void Initialize() override;
		void Update() override;
    };

	inline CMovespeedFeature* g_CMovespeedFeature = nullptr;
}