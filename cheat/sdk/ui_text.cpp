#include "pch.h"
#include "ui_text.h"

namespace SDK {
	UR::Class* UIText::GetClass() {
		if (!pClass) {
			pClass = UR::Get("UI.Beyond.dll")->Get("UIText");
		}
		return pClass;
	}
	void UIText::Initialize() {
		if (initialized)
			return;
		if (!GetClass()) {
			LOG("[-] UIText class not found!\n");
			return;
		}

		get_text = GetClass()->Get<UR::Method>("get_text");
		set_text = GetClass()->Get<UR::Method>("set_text", { "System.String" });

		initialized = true;
		LOG("[+] UIText initialized.\n");
		LOG("[*] UIText::get_text: %p\n", get_text->function);
		LOG("[*] UIText::set_text: %p\n", set_text->function);


	}
} // namespace SDK