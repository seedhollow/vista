#pragma once
#include "../feature_base.h"
namespace Features {
    class CAutoKillFeature : public IFeature {
    public:
        void Initialize() override;
    };

	inline CAutoKillFeature* g_AutoKillFeature = nullptr;
}