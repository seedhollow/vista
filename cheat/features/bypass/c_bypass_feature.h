#pragma once
#include "../feature_base.h"

namespace Features {
    class CBypassFeature : public IFeature {
    public:
        void Initialize() override;
    };

    inline CBypassFeature* g_BypassFeature = nullptr;
}