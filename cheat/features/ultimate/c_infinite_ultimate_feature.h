#pragma once

#include "../feature_base.h"

namespace Features {
    class CInfiniteUltimateFeature : public IFeature {
    public:
        void Initialize() override;
		void Update() override;
    };

    inline CInfiniteUltimateFeature* g_CInfiniteUltimateFeature = nullptr;
}