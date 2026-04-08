#include "pch.h"
#include "transform.h"

namespace SDK {
	UR::Class* Transform::GetClass() {
		if (!pClass) {
			pClass = UR::Get("UnityEngine.CoreModule.dll")->Get("Transform");
		}
		return pClass;
	}

	void Transform::Initialize() {
		if (initialized)
			return;
		if (!GetClass()) {
			LOG("[-] Transform class not found!\n");
			return;
		}
		get_position = GetClass()->Get<UR::Method>("get_position");
		set_position = GetClass()->Get<UR::Method>("set_position", { "UnityEngine.Vector3" });
		get_rotation = GetClass()->Get<UR::Method>("get_rotation");
		set_rotation = GetClass()->Get<UR::Method>("set_rotation", { "UnityEngine.Quaternion" });

		LOG("[+] Transform initialized.\n");
		LOG("[*] Transform::get_position: %p\n", get_position->function);
		LOG("[*] Transform::set_position: %p\n", set_position->function);
		LOG("[*] Transform::get_rotation: %p\n", get_rotation->function);
		LOG("[*] Transform::set_rotation: %p\n", set_rotation->function);
		initialized = true;
	}
} // namespace SDK