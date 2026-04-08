#pragma once
#include "../feature_base.h"

namespace Features {
    class CUncensoredFeature : public IFeature {
    public:
        void Initialize() override;
    };

    inline CUncensoredFeature* g_CUncensoredFeature = nullptr;
}