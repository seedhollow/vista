#pragma once
#include "../feature_base.h"

namespace Features {
    class CGodModeFeature : public IFeature {
    public:
        void Initialize() override;
        void Update() override;
    };

    inline CGodModeFeature* g_CGodModeFeature = nullptr;
}