#pragma once

#include "../feature_base.h"

namespace Features {
    class CEntityFinderFeature : public IFeature {
    public:
        void Initialize() override;
		void Update() override;
    };

	inline CEntityFinderFeature* g_CEntityFinderFeature = nullptr;
}