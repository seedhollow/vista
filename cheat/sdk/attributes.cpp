#include "pch.h"
#include "attributes.h"

namespace SDK {
	UR::Class* Attributes::GetClass() {
		if (!pClass) {
			pClass = UR::Get("Gameplay.Beyond.dll")->Get("Attributes");
		}
		return pClass;
	}

	void Attributes::Initialize( ) {
		if (initialized)
			return;

		if (!GetClass()) {
			LOG("[-] Attributes class not found!\n");
			return;
		}
		// Implementation for Attributes initialization

		GetValue = GetClass()->Get<UR::Method>("GetValue", {"AttributeType"})->function;
		LOG("[+] Attributes initialized.\n");
		LOG("[*] Attributes::GetValue: %p\n", GetValue);


		initialized = true;
	}
} // namespace SDK
