#include "pch.h"
#include "skeletal_morph_component.h"

namespace SDK {
	UR::Class* SkeletalMorphComponent::GetClass() {
		if (!pClass) {
			pClass = UR::Get("Gameplay.Beyond.dll")->Get("SkeletalMorphComponent");
		}
		return pClass;
	}

	void SkeletalMorphComponent::Initialize() {
		if (initialized)
			return;
		if (!GetClass()) {
			LOG("[-] SkeletalMorphComponent class not found!\n");
			return;
		}
		f_m_animator = GetClass()->Get<UR::Field>("m_animator");
		f_m_core	 = GetClass()->Get<UR::Field>("m_core");

		LOG("[+] SkeletalMorphComponent initialized.\n");
		LOG("[*] SkeletalMorphComponent::f_m_animator: %p\n", f_m_animator->offset);
		LOG("[*] SkeletalMorphComponent::f_m_core: %p\n", f_m_core->offset);
		initialized = true;
	}


} // namespace SDK