#include "pch.h"
#include "animator.h"

namespace SDK {
	UR::Class* Animator::GetClass() {
		if (!pClass) {
			pClass = UR::Get("UnityEngine.AnimationModule.dll")->Get("Animator");
		}
		return pClass;
	}

	void Animator::Initialize() {
		if (initialized)
			return;

		if (!GetClass()) {
			LOG("[-] Animator class not found!\n");
			return;
		}
		GetBoneTransform			= GetClass()->Get<UR::Method>("GetBoneTransform", { "UnityEngine.HumanBodyBones" });
		GetBoneTransformInternal	= GetClass()->Get<UR::Method>("GetBoneTransformInternal", { "System.Int32" });

		LOG("[+] Animator initialized.\n");
		LOG("[*] Animator::GetBoneTransform: %p\n", GetBoneTransform->function);
		LOG("[*] Animator::GetBoneTransformInternal: %p\n", GetBoneTransformInternal->function);
		initialized = true;
	}
} 