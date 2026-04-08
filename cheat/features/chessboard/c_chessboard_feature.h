#pragma once
#include "../feature_base.h"

namespace Features {
    class CChessboardFeature : public IFeature {
    public:
        void Initialize() override;
    };

	inline CChessboardFeature* g_CChessboardFeature = nullptr;
}