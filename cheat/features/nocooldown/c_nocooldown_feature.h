#pragma once

#include "../feature_base.h"

namespace Features {
    class CNocooldownFeature : public IFeature {
    public:
        void Initialize() override;

    };

    inline CNocooldownFeature* g_CNocooldownFeature = nullptr;
}