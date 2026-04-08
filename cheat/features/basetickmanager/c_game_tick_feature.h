#pragma once
#include "../feature_base.h"

namespace Features {
    class CGameTickFeature : public IFeature {
    public:
        void Initialize() override;
    };

	inline CGameTickFeature* g_CGameTickFeature = nullptr;
}