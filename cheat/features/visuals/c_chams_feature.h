#pragma once
#include "../feature_base.h"
namespace Features {
	using UT = UnityResolve::UnityType;
	class CChamsFeature : public IFeature {

	public:
		void Initialize() override;
		void Render() override;
		void Update() override;
	private:
		void DrawChams(void* entity);

	};
	inline CChamsFeature* g_ChamsFeature = nullptr;
}