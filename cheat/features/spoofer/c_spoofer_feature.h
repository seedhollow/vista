#pragma once

#include "../feature_base.h"

namespace Features {
	class CSpooferFeature : public IFeature {
	public:
		void Initialize() override;
		void Update() override;

	private:
		bool is_current_uid_retrieved = false;

		void apply_uid(UnityResolve::UnityType::GameObject* game_object, const char* uid);
	};
	inline CSpooferFeature* g_CSpooferFeature = nullptr;
}