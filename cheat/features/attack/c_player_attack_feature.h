#pragma once
#include "../feature_base.h"

namespace Features {
    class CPlayerAttackFeature : public IFeature {
    public:
        void Initialize() override;
		void Update() override;

    private:
		void AttackSpeed();
    };

    inline CPlayerAttackFeature* g_CPlayerAttackFeature = nullptr;
}