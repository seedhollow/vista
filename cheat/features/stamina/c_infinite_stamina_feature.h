#pragma once
#include "../feature_base.h"

namespace Features {
    class CInfiniteStaminaFeature : public IFeature {
    public:
        void Initialize() override;
    };

    inline CInfiniteStaminaFeature* g_CInfiniteStaminaFeature = nullptr;
}