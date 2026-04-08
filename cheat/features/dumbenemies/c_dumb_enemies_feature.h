#pragma once

#include "../feature_base.h"

namespace Features {
    class CDumbEnemiesFeature : public IFeature {
    public:
        void Initialize() override;

    };

    inline CDumbEnemiesFeature* g_CDumbEnemiesFeature = nullptr;
}