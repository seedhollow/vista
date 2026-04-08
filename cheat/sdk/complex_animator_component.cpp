#include "pch.h"
#include "complex_animator_component.h"

namespace SDK {
	UR::Class* ComplexAnimatorComponent::GetClass() {
		if (!pClass) {
			pClass = UR::Get("Gameplay.Beyond.dll")->Get("ComplexAnimatorComponent");
		}

		return pClass;
	}


	void ComplexAnimatorComponent::Initialize() {
		if (initialized)
			return;
		if (!GetClass()) {
			LOG("[-] ComplexAnimatorComponent class not found!\n");
		}

		get_animator = GetClass()->Get<UR::Method>("get_animator");

		initialized = true;

		LOG("[+] ComplexAnimatorComponent initialized.\n");
		LOG("[*] ComplexAnimatorComponent::get_animator: %p\n", get_animator->function);
	}


}// namespace SDK