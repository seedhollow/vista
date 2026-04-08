#include "pch.h"
#include "play_cutscene_action.h"

namespace SDK {
	UR::Class* PlayCutsceneAction::GetClass() {
		if (!pClass) pClass = UR::Get("Gameplay.Beyond.dll")->Get("PlayCutsceneAction");
		return pClass;
	}

	void PlayCutsceneAction::Initialize() {
		if (initialized) return;

		if (!GetClass()) {
			LOG("[-] PlayCutsceneAction class not found!\n");
			return;
		}

		Execute = pClass->Get<UR::Method>("Execute", { "System.Single" });

		initialized = true;
		LOG("[+] PlayCutsceneAction initialized.\n");
		LOG("[*] PlayCutsceneAction::Execute: %p\n", Execute->function);
	}
} // namespace SDK