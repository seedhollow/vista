#include "pch.h"
#include "c_camera_fov_feature.h"

namespace Features {

	static void hk_Camera_set_fieldOfView(void* __this, float fov) {
		if (misc_func->camera_fov) {
			fov = misc_func->camera_fov_value;
		}

		CALL_ORIGIN(hk_Camera_set_fieldOfView, __this, fov);
	}

	void CCameraFovFeature::Initialize() {
		LOG("[+] CCameraFovFeature initialized\n");
		HookManager::install(UnityResolve::Get("UnityEngine.CoreModule.dll")->Get("Camera")->Get<UnityResolve::Method>("set_fieldOfView")->function, hk_Camera_set_fieldOfView);
	}
}